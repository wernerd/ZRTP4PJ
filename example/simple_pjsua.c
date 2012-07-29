/* $Id$ */
/*
 * Copyright (C) 2011 Werner Dittmann <cwWerner.Dittmann@t-online.de>
 * Copyright (C) 2008-2009 Teluu Inc. (http://www.teluu.com)
 * Copyright (C) 2003-2008 Benny Prijono <benny@prijono.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/**
 * simple_pjsua.c
 *
 * This is a very simple but fully featured SIP user agent, with the
 * following capabilities:
 *  - SIP registration
 *  - Making and receiving call
 *  - Audio/media to sound device.
 *
 * Usage:
 *  - To make outgoing call, start simple_pjsua with the URL of remote
 *    destination to contact.
 *    E.g.:
 *   simpleua sip:user@remote
 *
 *  - Incoming calls will automatically be answered with 200.
 *
 * This program will quit once it has completed a single call.
 *
 * Modified by Werner Dittmann to show how to use ZRTP together with
 * pjsua. Use this only after you included the patch to pjsip that
 * enable the transport related callback (see patch file in top directory)
 */

#include <transport_zrtp.h>
#include <pjsua-lib/pjsua.h>

#define THIS_FILE   "APP"

/* Modify this according to your SIP settings */
#define SIP_DOMAIN  "werner.home"
#define SIP_USER    "100"
#define SIP_PASSWD  "secret"
#define SIP_PORT    5060
#define SIP_OUTBOUND_PROXY "sip:192.168.10.1:5070"



const char* InfoCodes[] =
{
    "EMPTY",
    "Hello received, preparing a Commit",
    "Commit: Generated a public DH key",
    "Responder: Commit received, preparing DHPart1",
    "DH1Part: Generated a public DH key",
    "Initiator: DHPart1 received, preparing DHPart2",
    "Responder: DHPart2 received, preparing Confirm1",
    "Initiator: Confirm1 received, preparing Confirm2",
    "Responder: Confirm2 received, preparing Conf2Ack",
    "At least one retained secrets matches - security OK",
    "Entered secure state",
    "No more security for this session"
};

/**
 * Sub-codes for Warning
 */
const char* WarningCodes [] =
{
    "EMPTY",
    "Commit contains an AES256 cipher but does not offer a Diffie-Helman 4096",
    "Received a GoClear message",
    "Hello offers an AES256 cipher but does not offer a Diffie-Helman 4096",
    "No retained shared secrets available - must verify SAS",
    "Internal ZRTP packet checksum mismatch - packet dropped",
    "Dropping packet because SRTP authentication failed!",
    "Dropping packet because SRTP replay check failed!",
    "Valid retained shared secrets availabe but no matches found - must verify SAS"
};

/**
 * Sub-codes for Severe
 */
const char* SevereCodes[] =
{
    "EMPTY",
    "Hash HMAC check of Hello failed!",
    "Hash HMAC check of Commit failed!",
    "Hash HMAC check of DHPart1 failed!",
    "Hash HMAC check of DHPart2 failed!",
    "Cannot send data - connection or peer down?",
    "Internal protocol error occured!",
    "Cannot start a timer - internal resources exhausted?",
    "Too much retries during ZRTP negotiation - connection or peer down?"
};

static void secureOn(void* data, char* cipher)
{
    PJ_LOG(3,(THIS_FILE, "Security enabled, cipher: %s", cipher));
}
static void secureOff(void* data)
{
    PJ_LOG(3,(THIS_FILE, "Security disabled"));
}
static void showSAS(void* data, char* sas, int32_t verified)
{
    PJ_LOG(3,(THIS_FILE, "SAS data: %s, verified: %d", sas, verified));
}
static void confirmGoClear(void* data)
{
    PJ_LOG(3,(THIS_FILE, "GoClear????????"));
}
static void showMessage(void* data, int32_t sev, int32_t subCode)
{
    switch (sev)
    {
        case zrtp_Info:
            PJ_LOG(3,(THIS_FILE, "ZRTP info message: %s", InfoCodes[subCode]));
            break;
            
        case zrtp_Warning:
            PJ_LOG(3,(THIS_FILE, "ZRTP warning message: %s", WarningCodes[subCode]));
            break;
            
        case zrtp_Severe:
            PJ_LOG(3,(THIS_FILE, "ZRTP severe message: %s", SevereCodes[subCode]));
            break;
            
        case zrtp_ZrtpError:
            PJ_LOG(3,(THIS_FILE, "ZRTP Error: severity: %d, subcode: %x", sev, subCode));
            break;
    }
}
static void zrtpNegotiationFailed(void* data, int32_t severity, int32_t subCode)
{
    PJ_LOG(3,(THIS_FILE, "ZRTP failed: %d, subcode: %d", severity, subCode));
}
static void zrtpNotSuppOther(void* data)
{
    PJ_LOG(3,(THIS_FILE, "ZRTP not supported by other peer"));
}
static void zrtpAskEnrollment(void* data, int32_t info)
{
    PJ_LOG(3,(THIS_FILE, "ZRTP - Ask PBX enrollment"));
}
static void zrtpInformEnrollment(void* data, int32_t info)
{
    PJ_LOG(3,(THIS_FILE, "ZRTP - Inform PBX enrollement"));
}
static void signSAS(void* data, unsigned char* sas)
{
    PJ_LOG(3,(THIS_FILE, "ZRTP - sign SAS"));
}
static int32_t checkSASSignature(void* data, unsigned char* sas)
{
    PJ_LOG(3,(THIS_FILE, "ZRTP - check SAS signature"));
}

static zrtp_UserCallbacks usercb =
{
    &secureOn,
    &secureOff,
    &showSAS,
    &confirmGoClear,
    &showMessage,
    &zrtpNegotiationFailed,
    &zrtpNotSuppOther,
    &zrtpAskEnrollment,
    &zrtpInformEnrollment,
    &signSAS,
    &checkSASSignature,
    NULL
};

/* Initialize the ZRTP transport and the user callbacks */
pjmedia_transport* on_create_media_transport(pjsua_call_id call_id,
                                             unsigned media_idx,
                                             pjmedia_transport *base_tp,
                                             unsigned flags)
{
    pjmedia_transport *zrtp_tp = NULL;
    pj_status_t status;
    pjmedia_endpt* endpt = pjsua_get_pjmedia_endpt();
    
    PJ_LOG(3,(THIS_FILE, "ZRTP transport created"));
    status = pjmedia_transport_zrtp_create(endpt, NULL, base_tp,
                                           &zrtp_tp, flags);
    usercb.userData = zrtp_tp;
    
    /* this is optional but highly recommended to enable the application
     * to report status information to the user, such as verfication status,
     * SAS code, etc
     */
    pjmedia_transport_zrtp_setUserCallback(zrtp_tp, &usercb);
    
    /*
     * Initialize the transport. Just the filename of the ZID file that holds
     * our partners ZID, shared data etc. If the files does not exists it will
     * be created an initialized.
     */
    pjmedia_transport_zrtp_initialize(zrtp_tp, "simple.zid", PJ_TRUE);
    return zrtp_tp;
}


/* Callback called by the library upon receiving incoming call */
static void on_incoming_call(pjsua_acc_id acc_id, pjsua_call_id call_id,
                             pjsip_rx_data *rdata)
{
    pjsua_call_info ci;
    
    PJ_UNUSED_ARG(acc_id);
    PJ_UNUSED_ARG(rdata);
    
    pjsua_call_get_info(call_id, &ci);
    
    PJ_LOG(3,(THIS_FILE, "Incoming call from %.*s!!",
              (int)ci.remote_info.slen,
              ci.remote_info.ptr));
    
    /* Automatically answer incoming calls with 200/OK */
    pjsua_call_answer(call_id, 200, NULL, NULL);
}

/* Callback called by the library when call's state has changed */
static void on_call_state(pjsua_call_id call_id, pjsip_event *e)
{
    pjsua_call_info ci;
    
    PJ_UNUSED_ARG(e);
    
    pjsua_call_get_info(call_id, &ci);
    PJ_LOG(3,(THIS_FILE, "Call %d state=%.*s", call_id,
              (int)ci.state_text.slen,
              ci.state_text.ptr));
}

/* Callback called by the library when call's media state has changed */
static void on_call_media_state(pjsua_call_id call_id)
{
    pjsua_call_info ci;
    
    pjsua_call_get_info(call_id, &ci);
    
    if (ci.media_status == PJSUA_CALL_MEDIA_ACTIVE)
    {
        // When media is active, connect call to sound device.
        pjsua_conf_connect(ci.conf_slot, 0);
        pjsua_conf_connect(0, ci.conf_slot);
    }
}

/* Display error and exit application */
static void error_exit(const char *title, pj_status_t status)
{
    pjsua_perror(THIS_FILE, title, status);
    pjsua_destroy();
    exit(1);
}

/*
 * main()
 *
 * argv[1] may contain URL to call.
 */
int main(int argc, char *argv[])
{
    pjsua_acc_id acc_id;
    pj_status_t status;
    
    /* Create pjsua first! */
    status = pjsua_create();
    if (status != PJ_SUCCESS) error_exit("Error in pjsua_create()", status);
    
    /* If argument is specified, it's got to be a valid SIP URL */
    if (argc > 1)
    {
        status = pjsua_verify_url(argv[1]);
        if (status != PJ_SUCCESS) error_exit("Invalid URL in argv", status);
    }
    
    /* Init pjsua */
    {
        pjsua_config cfg;
        pjsua_logging_config log_cfg;
        
        pjsua_config_default(&cfg);
        cfg.cb.on_incoming_call = &on_incoming_call;
        cfg.cb.on_call_media_state = &on_call_media_state;
        cfg.cb.on_call_state = &on_call_state;
        /*
         * Register the ZRTP created callback that sets up the ZRTP stuff
         *
         * This call is available only after you applied the patch to pjsip
         * (see top level directory)
         */
        cfg.cb.on_create_media_transport = &on_create_media_transport;
        cfg.outbound_proxy[cfg.outbound_proxy_cnt++] = pj_str(SIP_OUTBOUND_PROXY);
        
        pjsua_logging_config_default(&log_cfg);
        log_cfg.console_level = 4;
        
        status = pjsua_init(&cfg, &log_cfg, NULL);
        if (status != PJ_SUCCESS) error_exit("Error in pjsua_init()", status);
    }
    
    /* Add UDP transport. */
    {
        pjsua_transport_config cfg;
        
        pjsua_transport_config_default(&cfg);
        cfg.port = SIP_PORT;
        status = pjsua_transport_create(PJSIP_TRANSPORT_UDP, &cfg, NULL);
        if (status != PJ_SUCCESS) error_exit("Error creating transport", status);
    }
    
    /* Initialization is done, now start pjsua */
    status = pjsua_start();
    if (status != PJ_SUCCESS) error_exit("Error starting pjsua", status);
    
    /* Register to SIP server by creating SIP account. */
    {
        pjsua_acc_config cfg;
        
        printf("cred_info size: %d\n", sizeof(struct pjsip_cred_info));
        pjsua_acc_config_default(&cfg);
        cfg.id = pj_str("sip:" SIP_USER "@" SIP_DOMAIN);
        cfg.reg_uri = pj_str("sip:" SIP_DOMAIN);
        cfg.cred_count = 1;
        cfg.cred_info[0].realm = pj_str(SIP_DOMAIN);
        cfg.cred_info[0].scheme = pj_str("digest");
        cfg.cred_info[0].username = pj_str(SIP_USER);
        cfg.cred_info[0].data_type = PJSIP_CRED_DATA_PLAIN_PASSWD;
        cfg.cred_info[0].data = pj_str(SIP_PASSWD);
        
        status = pjsua_acc_add(&cfg, PJ_TRUE, &acc_id);
        if (status != PJ_SUCCESS) error_exit("Error adding account", status);
    }
    
    /* If URL is specified, make call to the URL. */
    if (argc > 1)
    {
        pj_str_t uri = pj_str(argv[1]);
        status = pjsua_call_make_call(acc_id, &uri, 0, NULL, NULL, NULL);
        if (status != PJ_SUCCESS) error_exit("Error making call", status);
    }
    
    /* Wait until user press "q" to quit. */
    for (;;)
    {
        char option[10];
        
        puts("Press 'h' to hangup all calls, 'q' to quit");
        if (fgets(option, sizeof(option), stdin) == NULL)
        {
            puts("EOF while reading stdin, will quit now..");
            break;
        }
        
        if (option[0] == 'q')
            break;
        
        if (option[0] == 'h')
            pjsua_call_hangup_all();
    }
    
    /* Destroy pjsua */
    pjsua_destroy();
    
    return 0;
}

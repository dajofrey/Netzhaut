#ifndef NH_HTML_MAIN_POLICY_CONTAINER_H
#define NH_HTML_MAIN_POLICY_CONTAINER_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

// 7.1.4
typedef enum NH_HTML_EMBEDDER_POLICY_E {
    NH_HTML_EMBEDDER_POLICY_UNSAFE_NONE,
    NH_HTML_EMBEDDER_POLICY_REQUIRE_CORP,
    NH_HTML_EMBEDDER_POLICY_CREDENTIALLESS,
} NH_HTML_EMBEDDER_POLICY_E;

// 7.1.4
typedef struct nh_html_EmbedderPolicy {
    NH_HTML_EMBEDDER_POLICY_E value;
    nh_encoding_UTF8 ReportingEndpoint;
    NH_HTML_EMBEDDER_POLICY_E reportOnlyValue;
    nh_encoding_UTF8 ReportOnlyReportingEndpoint;
} nh_html_EmbedderPolicy;

typedef enum NH_HTML_REFERRER_POLICY_E {
    NH_HTML_REFERRER_POLICY_NONE,
    NH_HTML_REFERRER_POLICY_NO_REFERRER,
    NH_HTML_REFERRER_POLICY_NO_REFERRER_WHEN_DOWNGRADE,
    NH_HTML_REFERRER_POLICY_SAME_ORIGIN,
    NH_HTML_REFERRER_POLICY_ORIGIN,
    NH_HTML_REFERRER_POLICY_STRICT_ORIGIN,
    NH_HTML_REFERRER_POLICY_ORIGIN_WHEN_CROSS_ORIGIN,
    NH_HTML_REFERRER_POLICY_STRICT_ORIGIN_WHEN_CROSS_ORIGIN,
    NH_HTML_REFERRER_POLICY_UNSAFE_URL,
} NH_HTML_REFERRER_POLICY_E;

// 7.1.6
typedef struct nh_html_PolicyContainer {
    // TODO CSP List
    nh_html_EmbedderPolicy EmbedderPolicy;
    NH_HTML_REFERRER_POLICY_E referrerPolicy;
} nh_html_PolicyContainer;

#endif

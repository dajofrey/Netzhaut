// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 *
 * This file was generated.
 */


// INCLUDE =========================================================================================

#include "Builtin.h"

#include "../Specifications/DOM/TreeWalker.idl.inc"
#include "../Specifications/DOM/Text.idl.inc"
#include "../Specifications/DOM/StaticRange.idl.inc"
#include "../Specifications/DOM/Slottable.idl.inc"
#include "../Specifications/DOM/ShadowRoot.idl.inc"
#include "../Specifications/DOM/Range.idl.inc"
#include "../Specifications/DOM/ProcessingInstruction.idl.inc"
#include "../Specifications/DOM/ParentNode.idl.inc"
#include "../Specifications/DOM/NonElementParentNode.idl.inc"
#include "../Specifications/DOM/NonDocumentTypeChildNode.idl.inc"
#include "../Specifications/DOM/NodeList.idl.inc"
#include "../Specifications/DOM/NodeIterator.idl.inc"
#include "../Specifications/DOM/NodeFilter.idl.inc"
#include "../Specifications/DOM/Node.idl.inc"
#include "../Specifications/DOM/NamedNodeMap.idl.inc"
#include "../Specifications/DOM/MutationRecord.idl.inc"
#include "../Specifications/DOM/MutationObserver.idl.inc"
#include "../Specifications/DOM/HTMLCollection.idl.inc"
#include "../Specifications/DOM/EventTarget.idl.inc"
#include "../Specifications/DOM/Event.idl.inc"
#include "../Specifications/DOM/Element.idl.inc"
#include "../Specifications/DOM/DocumentType.idl.inc"
#include "../Specifications/DOM/DocumentOrShadowRoot.idl.inc"
#include "../Specifications/DOM/DocumentFragment.idl.inc"
#include "../Specifications/DOM/Document.idl.inc"
#include "../Specifications/DOM/DOMTokenList.idl.inc"
#include "../Specifications/DOM/DOMImplementation.idl.inc"
#include "../Specifications/DOM/Comment.idl.inc"
#include "../Specifications/DOM/ChildNode.idl.inc"
#include "../Specifications/DOM/CharacterData.idl.inc"
#include "../Specifications/DOM/CDataSection.idl.inc"
#include "../Specifications/DOM/Attr.idl.inc"
#include "../Specifications/DOM/AbstractRange.idl.inc"
#include "../Specifications/DOM/AbortSignal.idl.inc"
#include "../Specifications/DOM/AbortController.idl.inc"
#include "../Specifications/HTML/AbstractWorker.idl.inc"
#include "../Specifications/HTML/HTMLDataElement.idl.inc"
#include "../Specifications/HTML/HTMLLabelElement.idl.inc"
#include "../Specifications/HTML/HTMLParamElement.idl.inc"
#include "../Specifications/HTML/HTMLTextAreaElement.idl.inc"
#include "../Specifications/HTML/Storage.idl.inc"
#include "../Specifications/HTML/AudioAndVideoTrackList.idl.inc"
#include "../Specifications/HTML/HTMLDataListElement.idl.inc"
#include "../Specifications/HTML/HTMLLegendElement.idl.inc"
#include "../Specifications/HTML/HTMLPictureElement.idl.inc"
#include "../Specifications/HTML/HTMLTimeElement.idl.inc"
#include "../Specifications/HTML/SubmitEvent.idl.inc"
#include "../Specifications/HTML/CanvasRenderingContext2D.idl.inc"
#include "../Specifications/HTML/HTMLDetailsElement.idl.inc"
#include "../Specifications/HTML/HTMLLIElement.idl.inc"
#include "../Specifications/HTML/HTMLPreElement.idl.inc"
#include "../Specifications/HTML/HTMLTitleElement.idl.inc"
#include "../Specifications/HTML/TextTrackCue.idl.inc"
#include "../Specifications/HTML/CustomElementRegistry.idl.inc"
#include "../Specifications/HTML/HTMLDialogElement.idl.inc"
#include "../Specifications/HTML/HTMLLinkElement.idl.inc"
#include "../Specifications/HTML/HTMLProgressElement.idl.inc"
#include "../Specifications/HTML/HTMLTrackElement.idl.inc"
#include "../Specifications/HTML/TextTrackCueList.idl.inc"
#include "../Specifications/HTML/DedicatedWorkerGlobalScope.idl.inc"
#include "../Specifications/HTML/HTMLDivElement.idl.inc"
#include "../Specifications/HTML/HTMLMapElement.idl.inc"
#include "../Specifications/HTML/HTMLQuoteElement.idl.inc"
#include "../Specifications/HTML/HTMLUListElement.idl.inc"
#include "../Specifications/HTML/TextTrack.idl.inc"
#include "../Specifications/HTML/Document.idl.inc"
#include "../Specifications/HTML/HTMLDListElement.idl.inc"
#include "../Specifications/HTML/HTMLMediaElement.idl.inc"
#include "../Specifications/HTML/HTMLScriptElement.idl.inc"
#include "../Specifications/HTML/HTMLUnknownElement.idl.inc"
#include "../Specifications/HTML/TextTrackList.idl.inc"
#include "../Specifications/HTML/DocumentOrShadowRoot.idl.inc"
#include "../Specifications/HTML/HTMLElement.idl.inc"
#include "../Specifications/HTML/HTMLMediaError.idl.inc"
#include "../Specifications/HTML/HTMLSelectElement.idl.inc"
#include "../Specifications/HTML/HTMLVideoElement.idl.inc"
#include "../Specifications/HTML/TimeRanges.idl.inc"
#include "../Specifications/HTML/DOMStringMap.idl.inc"
#include "../Specifications/HTML/HTMLEmbedElement.idl.inc"
#include "../Specifications/HTML/HTMLMenuElement.idl.inc"
#include "../Specifications/HTML/HTMLSlotElement.idl.inc"
#include "../Specifications/HTML/ImageBitmapRenderingContext.idl.inc"
#include "../Specifications/HTML/TrackEvent.idl.inc"
#include "../Specifications/HTML/ElementInternals.idl.inc"
#include "../Specifications/HTML/HTMLFieldSetElement.idl.inc"
#include "../Specifications/HTML/HTMLMetaElement.idl.inc"
#include "../Specifications/HTML/HTMLSourceElement.idl.inc"
#include "../Specifications/HTML/LocalStorage.idl.inc"
#include "../Specifications/HTML/ValidityState.idl.inc"
#include "../Specifications/HTML/FormDataEvent.idl.inc"
#include "../Specifications/HTML/HTMLFormElement.idl.inc"
#include "../Specifications/HTML/HTMLMeterElement.idl.inc"
#include "../Specifications/HTML/HTMLSpanElement.idl.inc"
#include "../Specifications/HTML/MessageEvent.idl.inc"
#include "../Specifications/HTML/WebSocket.idl.inc"
#include "../Specifications/HTML/HTMLAnchorElement.idl.inc"
#include "../Specifications/HTML/HTMLHeadElement.idl.inc"
#include "../Specifications/HTML/HTMLModElement.idl.inc"
#include "../Specifications/HTML/HTMLStyleElement.idl.inc"
#include "../Specifications/HTML/NavigatorConcurrentHardware.idl.inc"
#include "../Specifications/HTML/Window.idl.inc"
#include "../Specifications/HTML/HTMLAreaElement.idl.inc"
#include "../Specifications/HTML/HTMLHeadingElement.idl.inc"
#include "../Specifications/HTML/HTMLObjectElement.idl.inc"
#include "../Specifications/HTML/HTMLTableCaptionElement.idl.inc"
#include "../Specifications/HTML/OffscreenCanvas.idl.inc"
#include "../Specifications/HTML/WindowProxy.idl.inc"
#include "../Specifications/HTML/HTMLAudioElement.idl.inc"
#include "../Specifications/HTML/HTMLHRElement.idl.inc"
#include "../Specifications/HTML/HTMLOListElement.idl.inc"
#include "../Specifications/HTML/HTMLTableCellElement.idl.inc"
#include "../Specifications/HTML/OffscreenCanvasRenderingContext2D.idl.inc"
#include "../Specifications/HTML/WorkerGlobalScope.idl.inc"
#include "../Specifications/HTML/HTMLBaseElement.idl.inc"
#include "../Specifications/HTML/HTMLHtmlElement.idl.inc"
#include "../Specifications/HTML/HTMLOptGroupElement.idl.inc"
#include "../Specifications/HTML/HTMLTableColElement.idl.inc"
#include "../Specifications/HTML/SelectionMode.idl.inc"
#include "../Specifications/HTML/Worker.idl.inc"
#include "../Specifications/HTML/HTMLBodyElement.idl.inc"
#include "../Specifications/HTML/HTMLHyperlinkElementUtils.idl.inc"
#include "../Specifications/HTML/HTMLOptionElement.idl.inc"
#include "../Specifications/HTML/HTMLTableElement.idl.inc"
#include "../Specifications/HTML/SessionStorage.idl.inc"
#include "../Specifications/HTML/WorkerLocation.idl.inc"
#include "../Specifications/HTML/HTMLBRElement.idl.inc"
#include "../Specifications/HTML/HTMLIFrameElement.idl.inc"
#include "../Specifications/HTML/HTMLOrSVGElement.idl.inc"
#include "../Specifications/HTML/HTMLTableRowElement.idl.inc"
#include "../Specifications/HTML/SharedWorkerGlobalScope.idl.inc"
#include "../Specifications/HTML/WorkerNavigator.idl.inc"
#include "../Specifications/HTML/HTMLButtonElement.idl.inc"
#include "../Specifications/HTML/HTMLImageElement.idl.inc"
#include "../Specifications/HTML/HTMLOutputElement.idl.inc"
#include "../Specifications/HTML/HTMLTableSectionElement.idl.inc"
#include "../Specifications/HTML/SharedWorker.idl.inc"
#include "../Specifications/HTML/WorkletGlobalScope.idl.inc"
#include "../Specifications/HTML/HTMLCanvasElement.idl.inc"
#include "../Specifications/HTML/HTMLInputElement.idl.inc"
#include "../Specifications/HTML/HTMLParagraphElement.idl.inc"
#include "../Specifications/HTML/HTMLTemplateElement.idl.inc"
#include "../Specifications/HTML/StorageEvent.idl.inc"
#include "../Specifications/HTML/Worklet.idl.inc"
#include "../Specifications/CSS/CSSGroupingRule.idl.inc"
#include "../Specifications/CSS/CSSMarginRule.idl.inc"
#include "../Specifications/CSS/CSSPageRule.idl.inc"
#include "../Specifications/CSS/CSSRuleList.idl.inc"
#include "../Specifications/CSS/CSSStyleRule.idl.inc"
#include "../Specifications/CSS/Document.idl.inc"
#include "../Specifications/CSS/StyleSheetList.idl.inc"
#include "../Specifications/CSS/CSSImportRule.idl.inc"
#include "../Specifications/CSS/CSSNamespaceRule.idl.inc"
#include "../Specifications/CSS/CSSRule.idl.inc"
#include "../Specifications/CSS/CSSStyleDeclaration.idl.inc"
#include "../Specifications/CSS/CSSStyleSheet.idl.inc"
#include "../Specifications/CSS/CSSCounterStyleRule.idl.inc"
#include "../Specifications/CSS/StyleSheet.idl.inc"
#include "../Specifications/URL/URL.idl.inc"
#include "../Specifications/URL/URLSearchParams.idl.inc"

// FRAGMENT_NAMES ==================================================================================

char *NH_WEBIDL_FRAGMENT_NAMES_PP[] = {
    "DOM_TreeWalker",
    "DOM_Text",
    "DOM_StaticRange",
    "DOM_Slottable",
    "DOM_ShadowRoot",
    "DOM_Range",
    "DOM_ProcessingInstruction",
    "DOM_ParentNode",
    "DOM_NonElementParentNode",
    "DOM_NonDocumentTypeChildNode",
    "DOM_NodeList",
    "DOM_NodeIterator",
    "DOM_NodeFilter",
    "DOM_Node",
    "DOM_NamedNodeMap",
    "DOM_MutationRecord",
    "DOM_MutationObserver",
    "DOM_HTMLCollection",
    "DOM_EventTarget",
    "DOM_Event",
    "DOM_Element",
    "DOM_DocumentType",
    "DOM_DocumentOrShadowRoot",
    "DOM_DocumentFragment",
    "DOM_Document",
    "DOM_DOMTokenList",
    "DOM_DOMImplementation",
    "DOM_Comment",
    "DOM_ChildNode",
    "DOM_CharacterData",
    "DOM_CDataSection",
    "DOM_Attr",
    "DOM_AbstractRange",
    "DOM_AbortSignal",
    "DOM_AbortController",
    "HTML_AbstractWorker",
    "HTML_HTMLDataElement",
    "HTML_HTMLLabelElement",
    "HTML_HTMLParamElement",
    "HTML_HTMLTextAreaElement",
    "HTML_Storage",
    "HTML_AudioAndVideoTrackList",
    "HTML_HTMLDataListElement",
    "HTML_HTMLLegendElement",
    "HTML_HTMLPictureElement",
    "HTML_HTMLTimeElement",
    "HTML_SubmitEvent",
    "HTML_CanvasRenderingContext2D",
    "HTML_HTMLDetailsElement",
    "HTML_HTMLLIElement",
    "HTML_HTMLPreElement",
    "HTML_HTMLTitleElement",
    "HTML_TextTrackCue",
    "HTML_CustomElementRegistry",
    "HTML_HTMLDialogElement",
    "HTML_HTMLLinkElement",
    "HTML_HTMLProgressElement",
    "HTML_HTMLTrackElement",
    "HTML_TextTrackCueList",
    "HTML_DedicatedWorkerGlobalScope",
    "HTML_HTMLDivElement",
    "HTML_HTMLMapElement",
    "HTML_HTMLQuoteElement",
    "HTML_HTMLUListElement",
    "HTML_TextTrack",
    "HTML_Document",
    "HTML_HTMLDListElement",
    "HTML_HTMLMediaElement",
    "HTML_HTMLScriptElement",
    "HTML_HTMLUnknownElement",
    "HTML_TextTrackList",
    "HTML_DocumentOrShadowRoot",
    "HTML_HTMLElement",
    "HTML_HTMLMediaError",
    "HTML_HTMLSelectElement",
    "HTML_HTMLVideoElement",
    "HTML_TimeRanges",
    "HTML_DOMStringMap",
    "HTML_HTMLEmbedElement",
    "HTML_HTMLMenuElement",
    "HTML_HTMLSlotElement",
    "HTML_ImageBitmapRenderingContext",
    "HTML_TrackEvent",
    "HTML_ElementInternals",
    "HTML_HTMLFieldSetElement",
    "HTML_HTMLMetaElement",
    "HTML_HTMLSourceElement",
    "HTML_LocalStorage",
    "HTML_ValidityState",
    "HTML_FormDataEvent",
    "HTML_HTMLFormElement",
    "HTML_HTMLMeterElement",
    "HTML_HTMLSpanElement",
    "HTML_MessageEvent",
    "HTML_WebSocket",
    "HTML_HTMLAnchorElement",
    "HTML_HTMLHeadElement",
    "HTML_HTMLModElement",
    "HTML_HTMLStyleElement",
    "HTML_NavigatorConcurrentHardware",
    "HTML_Window",
    "HTML_HTMLAreaElement",
    "HTML_HTMLHeadingElement",
    "HTML_HTMLObjectElement",
    "HTML_HTMLTableCaptionElement",
    "HTML_OffscreenCanvas",
    "HTML_WindowProxy",
    "HTML_HTMLAudioElement",
    "HTML_HTMLHRElement",
    "HTML_HTMLOListElement",
    "HTML_HTMLTableCellElement",
    "HTML_OffscreenCanvasRenderingContext2D",
    "HTML_WorkerGlobalScope",
    "HTML_HTMLBaseElement",
    "HTML_HTMLHtmlElement",
    "HTML_HTMLOptGroupElement",
    "HTML_HTMLTableColElement",
    "HTML_SelectionMode",
    "HTML_Worker",
    "HTML_HTMLBodyElement",
    "HTML_HTMLHyperlinkElementUtils",
    "HTML_HTMLOptionElement",
    "HTML_HTMLTableElement",
    "HTML_SessionStorage",
    "HTML_WorkerLocation",
    "HTML_HTMLBRElement",
    "HTML_HTMLIFrameElement",
    "HTML_HTMLOrSVGElement",
    "HTML_HTMLTableRowElement",
    "HTML_SharedWorkerGlobalScope",
    "HTML_WorkerNavigator",
    "HTML_HTMLButtonElement",
    "HTML_HTMLImageElement",
    "HTML_HTMLOutputElement",
    "HTML_HTMLTableSectionElement",
    "HTML_SharedWorker",
    "HTML_WorkletGlobalScope",
    "HTML_HTMLCanvasElement",
    "HTML_HTMLInputElement",
    "HTML_HTMLParagraphElement",
    "HTML_HTMLTemplateElement",
    "HTML_StorageEvent",
    "HTML_Worklet",
    "CSS_CSSGroupingRule",
    "CSS_CSSMarginRule",
    "CSS_CSSPageRule",
    "CSS_CSSRuleList",
    "CSS_CSSStyleRule",
    "CSS_Document",
    "CSS_StyleSheetList",
    "CSS_CSSImportRule",
    "CSS_CSSNamespaceRule",
    "CSS_CSSRule",
    "CSS_CSSStyleDeclaration",
    "CSS_CSSStyleSheet",
    "CSS_CSSCounterStyleRule",
    "CSS_StyleSheet",
    "URL_URL",
    "URL_URLSearchParams",
};

size_t NH_WEBIDL_FRAGMENT_NAMES_PP_COUNT = sizeof(NH_WEBIDL_FRAGMENT_NAMES_PP) / sizeof(NH_WEBIDL_FRAGMENT_NAMES_PP[0]);

// FRAGMENTS =======================================================================================

unsigned char *NH_WEBIDL_FRAGMENTS_PP[] = {
    nh_webidl_DOM_TreeWalker_p,
    nh_webidl_DOM_Text_p,
    nh_webidl_DOM_StaticRange_p,
    nh_webidl_DOM_Slottable_p,
    nh_webidl_DOM_ShadowRoot_p,
    nh_webidl_DOM_Range_p,
    nh_webidl_DOM_ProcessingInstruction_p,
    nh_webidl_DOM_ParentNode_p,
    nh_webidl_DOM_NonElementParentNode_p,
    nh_webidl_DOM_NonDocumentTypeChildNode_p,
    nh_webidl_DOM_NodeList_p,
    nh_webidl_DOM_NodeIterator_p,
    nh_webidl_DOM_NodeFilter_p,
    nh_webidl_DOM_Node_p,
    nh_webidl_DOM_NamedNodeMap_p,
    nh_webidl_DOM_MutationRecord_p,
    nh_webidl_DOM_MutationObserver_p,
    nh_webidl_DOM_HTMLCollection_p,
    nh_webidl_DOM_EventTarget_p,
    nh_webidl_DOM_Event_p,
    nh_webidl_DOM_Element_p,
    nh_webidl_DOM_DocumentType_p,
    nh_webidl_DOM_DocumentOrShadowRoot_p,
    nh_webidl_DOM_DocumentFragment_p,
    nh_webidl_DOM_Document_p,
    nh_webidl_DOM_DOMTokenList_p,
    nh_webidl_DOM_DOMImplementation_p,
    nh_webidl_DOM_Comment_p,
    nh_webidl_DOM_ChildNode_p,
    nh_webidl_DOM_CharacterData_p,
    nh_webidl_DOM_CDataSection_p,
    nh_webidl_DOM_Attr_p,
    nh_webidl_DOM_AbstractRange_p,
    nh_webidl_DOM_AbortSignal_p,
    nh_webidl_DOM_AbortController_p,
    nh_webidl_HTML_AbstractWorker_p,
    nh_webidl_HTML_HTMLDataElement_p,
    nh_webidl_HTML_HTMLLabelElement_p,
    nh_webidl_HTML_HTMLParamElement_p,
    nh_webidl_HTML_HTMLTextAreaElement_p,
    nh_webidl_HTML_Storage_p,
    nh_webidl_HTML_AudioAndVideoTrackList_p,
    nh_webidl_HTML_HTMLDataListElement_p,
    nh_webidl_HTML_HTMLLegendElement_p,
    nh_webidl_HTML_HTMLPictureElement_p,
    nh_webidl_HTML_HTMLTimeElement_p,
    nh_webidl_HTML_SubmitEvent_p,
    nh_webidl_HTML_CanvasRenderingContext2D_p,
    nh_webidl_HTML_HTMLDetailsElement_p,
    nh_webidl_HTML_HTMLLIElement_p,
    nh_webidl_HTML_HTMLPreElement_p,
    nh_webidl_HTML_HTMLTitleElement_p,
    nh_webidl_HTML_TextTrackCue_p,
    nh_webidl_HTML_CustomElementRegistry_p,
    nh_webidl_HTML_HTMLDialogElement_p,
    nh_webidl_HTML_HTMLLinkElement_p,
    nh_webidl_HTML_HTMLProgressElement_p,
    nh_webidl_HTML_HTMLTrackElement_p,
    nh_webidl_HTML_TextTrackCueList_p,
    nh_webidl_HTML_DedicatedWorkerGlobalScope_p,
    nh_webidl_HTML_HTMLDivElement_p,
    nh_webidl_HTML_HTMLMapElement_p,
    nh_webidl_HTML_HTMLQuoteElement_p,
    nh_webidl_HTML_HTMLUListElement_p,
    nh_webidl_HTML_TextTrack_p,
    nh_webidl_HTML_Document_p,
    nh_webidl_HTML_HTMLDListElement_p,
    nh_webidl_HTML_HTMLMediaElement_p,
    nh_webidl_HTML_HTMLScriptElement_p,
    nh_webidl_HTML_HTMLUnknownElement_p,
    nh_webidl_HTML_TextTrackList_p,
    nh_webidl_HTML_DocumentOrShadowRoot_p,
    nh_webidl_HTML_HTMLElement_p,
    nh_webidl_HTML_HTMLMediaError_p,
    nh_webidl_HTML_HTMLSelectElement_p,
    nh_webidl_HTML_HTMLVideoElement_p,
    nh_webidl_HTML_TimeRanges_p,
    nh_webidl_HTML_DOMStringMap_p,
    nh_webidl_HTML_HTMLEmbedElement_p,
    nh_webidl_HTML_HTMLMenuElement_p,
    nh_webidl_HTML_HTMLSlotElement_p,
    nh_webidl_HTML_ImageBitmapRenderingContext_p,
    nh_webidl_HTML_TrackEvent_p,
    nh_webidl_HTML_ElementInternals_p,
    nh_webidl_HTML_HTMLFieldSetElement_p,
    nh_webidl_HTML_HTMLMetaElement_p,
    nh_webidl_HTML_HTMLSourceElement_p,
    nh_webidl_HTML_LocalStorage_p,
    nh_webidl_HTML_ValidityState_p,
    nh_webidl_HTML_FormDataEvent_p,
    nh_webidl_HTML_HTMLFormElement_p,
    nh_webidl_HTML_HTMLMeterElement_p,
    nh_webidl_HTML_HTMLSpanElement_p,
    nh_webidl_HTML_MessageEvent_p,
    nh_webidl_HTML_WebSocket_p,
    nh_webidl_HTML_HTMLAnchorElement_p,
    nh_webidl_HTML_HTMLHeadElement_p,
    nh_webidl_HTML_HTMLModElement_p,
    nh_webidl_HTML_HTMLStyleElement_p,
    nh_webidl_HTML_NavigatorConcurrentHardware_p,
    nh_webidl_HTML_Window_p,
    nh_webidl_HTML_HTMLAreaElement_p,
    nh_webidl_HTML_HTMLHeadingElement_p,
    nh_webidl_HTML_HTMLObjectElement_p,
    nh_webidl_HTML_HTMLTableCaptionElement_p,
    nh_webidl_HTML_OffscreenCanvas_p,
    nh_webidl_HTML_WindowProxy_p,
    nh_webidl_HTML_HTMLAudioElement_p,
    nh_webidl_HTML_HTMLHRElement_p,
    nh_webidl_HTML_HTMLOListElement_p,
    nh_webidl_HTML_HTMLTableCellElement_p,
    nh_webidl_HTML_OffscreenCanvasRenderingContext2D_p,
    nh_webidl_HTML_WorkerGlobalScope_p,
    nh_webidl_HTML_HTMLBaseElement_p,
    nh_webidl_HTML_HTMLHtmlElement_p,
    nh_webidl_HTML_HTMLOptGroupElement_p,
    nh_webidl_HTML_HTMLTableColElement_p,
    nh_webidl_HTML_SelectionMode_p,
    nh_webidl_HTML_Worker_p,
    nh_webidl_HTML_HTMLBodyElement_p,
    nh_webidl_HTML_HTMLHyperlinkElementUtils_p,
    nh_webidl_HTML_HTMLOptionElement_p,
    nh_webidl_HTML_HTMLTableElement_p,
    nh_webidl_HTML_SessionStorage_p,
    nh_webidl_HTML_WorkerLocation_p,
    nh_webidl_HTML_HTMLBRElement_p,
    nh_webidl_HTML_HTMLIFrameElement_p,
    nh_webidl_HTML_HTMLOrSVGElement_p,
    nh_webidl_HTML_HTMLTableRowElement_p,
    nh_webidl_HTML_SharedWorkerGlobalScope_p,
    nh_webidl_HTML_WorkerNavigator_p,
    nh_webidl_HTML_HTMLButtonElement_p,
    nh_webidl_HTML_HTMLImageElement_p,
    nh_webidl_HTML_HTMLOutputElement_p,
    nh_webidl_HTML_HTMLTableSectionElement_p,
    nh_webidl_HTML_SharedWorker_p,
    nh_webidl_HTML_WorkletGlobalScope_p,
    nh_webidl_HTML_HTMLCanvasElement_p,
    nh_webidl_HTML_HTMLInputElement_p,
    nh_webidl_HTML_HTMLParagraphElement_p,
    nh_webidl_HTML_HTMLTemplateElement_p,
    nh_webidl_HTML_StorageEvent_p,
    nh_webidl_HTML_Worklet_p,
    nh_webidl_CSS_CSSGroupingRule_p,
    nh_webidl_CSS_CSSMarginRule_p,
    nh_webidl_CSS_CSSPageRule_p,
    nh_webidl_CSS_CSSRuleList_p,
    nh_webidl_CSS_CSSStyleRule_p,
    nh_webidl_CSS_Document_p,
    nh_webidl_CSS_StyleSheetList_p,
    nh_webidl_CSS_CSSImportRule_p,
    nh_webidl_CSS_CSSNamespaceRule_p,
    nh_webidl_CSS_CSSRule_p,
    nh_webidl_CSS_CSSStyleDeclaration_p,
    nh_webidl_CSS_CSSStyleSheet_p,
    nh_webidl_CSS_CSSCounterStyleRule_p,
    nh_webidl_CSS_StyleSheet_p,
    nh_webidl_URL_URL_p,
    nh_webidl_URL_URLSearchParams_p,
};

size_t NH_WEBIDL_FRAGMENTS_PP_COUNT = sizeof(NH_WEBIDL_FRAGMENTS_PP) / sizeof(NH_WEBIDL_FRAGMENTS_PP[0]);

// FRAGMENT LENGTHS ================================================================================

const unsigned int NH_WEBIDL_FRAGMENT_LENGTHS_P[] = {
    nh_webidl_DOM_TreeWalker_p_len,
    nh_webidl_DOM_Text_p_len,
    nh_webidl_DOM_StaticRange_p_len,
    nh_webidl_DOM_Slottable_p_len,
    nh_webidl_DOM_ShadowRoot_p_len,
    nh_webidl_DOM_Range_p_len,
    nh_webidl_DOM_ProcessingInstruction_p_len,
    nh_webidl_DOM_ParentNode_p_len,
    nh_webidl_DOM_NonElementParentNode_p_len,
    nh_webidl_DOM_NonDocumentTypeChildNode_p_len,
    nh_webidl_DOM_NodeList_p_len,
    nh_webidl_DOM_NodeIterator_p_len,
    nh_webidl_DOM_NodeFilter_p_len,
    nh_webidl_DOM_Node_p_len,
    nh_webidl_DOM_NamedNodeMap_p_len,
    nh_webidl_DOM_MutationRecord_p_len,
    nh_webidl_DOM_MutationObserver_p_len,
    nh_webidl_DOM_HTMLCollection_p_len,
    nh_webidl_DOM_EventTarget_p_len,
    nh_webidl_DOM_Event_p_len,
    nh_webidl_DOM_Element_p_len,
    nh_webidl_DOM_DocumentType_p_len,
    nh_webidl_DOM_DocumentOrShadowRoot_p_len,
    nh_webidl_DOM_DocumentFragment_p_len,
    nh_webidl_DOM_Document_p_len,
    nh_webidl_DOM_DOMTokenList_p_len,
    nh_webidl_DOM_DOMImplementation_p_len,
    nh_webidl_DOM_Comment_p_len,
    nh_webidl_DOM_ChildNode_p_len,
    nh_webidl_DOM_CharacterData_p_len,
    nh_webidl_DOM_CDataSection_p_len,
    nh_webidl_DOM_Attr_p_len,
    nh_webidl_DOM_AbstractRange_p_len,
    nh_webidl_DOM_AbortSignal_p_len,
    nh_webidl_DOM_AbortController_p_len,
    nh_webidl_HTML_AbstractWorker_p_len,
    nh_webidl_HTML_HTMLDataElement_p_len,
    nh_webidl_HTML_HTMLLabelElement_p_len,
    nh_webidl_HTML_HTMLParamElement_p_len,
    nh_webidl_HTML_HTMLTextAreaElement_p_len,
    nh_webidl_HTML_Storage_p_len,
    nh_webidl_HTML_AudioAndVideoTrackList_p_len,
    nh_webidl_HTML_HTMLDataListElement_p_len,
    nh_webidl_HTML_HTMLLegendElement_p_len,
    nh_webidl_HTML_HTMLPictureElement_p_len,
    nh_webidl_HTML_HTMLTimeElement_p_len,
    nh_webidl_HTML_SubmitEvent_p_len,
    nh_webidl_HTML_CanvasRenderingContext2D_p_len,
    nh_webidl_HTML_HTMLDetailsElement_p_len,
    nh_webidl_HTML_HTMLLIElement_p_len,
    nh_webidl_HTML_HTMLPreElement_p_len,
    nh_webidl_HTML_HTMLTitleElement_p_len,
    nh_webidl_HTML_TextTrackCue_p_len,
    nh_webidl_HTML_CustomElementRegistry_p_len,
    nh_webidl_HTML_HTMLDialogElement_p_len,
    nh_webidl_HTML_HTMLLinkElement_p_len,
    nh_webidl_HTML_HTMLProgressElement_p_len,
    nh_webidl_HTML_HTMLTrackElement_p_len,
    nh_webidl_HTML_TextTrackCueList_p_len,
    nh_webidl_HTML_DedicatedWorkerGlobalScope_p_len,
    nh_webidl_HTML_HTMLDivElement_p_len,
    nh_webidl_HTML_HTMLMapElement_p_len,
    nh_webidl_HTML_HTMLQuoteElement_p_len,
    nh_webidl_HTML_HTMLUListElement_p_len,
    nh_webidl_HTML_TextTrack_p_len,
    nh_webidl_HTML_Document_p_len,
    nh_webidl_HTML_HTMLDListElement_p_len,
    nh_webidl_HTML_HTMLMediaElement_p_len,
    nh_webidl_HTML_HTMLScriptElement_p_len,
    nh_webidl_HTML_HTMLUnknownElement_p_len,
    nh_webidl_HTML_TextTrackList_p_len,
    nh_webidl_HTML_DocumentOrShadowRoot_p_len,
    nh_webidl_HTML_HTMLElement_p_len,
    nh_webidl_HTML_HTMLMediaError_p_len,
    nh_webidl_HTML_HTMLSelectElement_p_len,
    nh_webidl_HTML_HTMLVideoElement_p_len,
    nh_webidl_HTML_TimeRanges_p_len,
    nh_webidl_HTML_DOMStringMap_p_len,
    nh_webidl_HTML_HTMLEmbedElement_p_len,
    nh_webidl_HTML_HTMLMenuElement_p_len,
    nh_webidl_HTML_HTMLSlotElement_p_len,
    nh_webidl_HTML_ImageBitmapRenderingContext_p_len,
    nh_webidl_HTML_TrackEvent_p_len,
    nh_webidl_HTML_ElementInternals_p_len,
    nh_webidl_HTML_HTMLFieldSetElement_p_len,
    nh_webidl_HTML_HTMLMetaElement_p_len,
    nh_webidl_HTML_HTMLSourceElement_p_len,
    nh_webidl_HTML_LocalStorage_p_len,
    nh_webidl_HTML_ValidityState_p_len,
    nh_webidl_HTML_FormDataEvent_p_len,
    nh_webidl_HTML_HTMLFormElement_p_len,
    nh_webidl_HTML_HTMLMeterElement_p_len,
    nh_webidl_HTML_HTMLSpanElement_p_len,
    nh_webidl_HTML_MessageEvent_p_len,
    nh_webidl_HTML_WebSocket_p_len,
    nh_webidl_HTML_HTMLAnchorElement_p_len,
    nh_webidl_HTML_HTMLHeadElement_p_len,
    nh_webidl_HTML_HTMLModElement_p_len,
    nh_webidl_HTML_HTMLStyleElement_p_len,
    nh_webidl_HTML_NavigatorConcurrentHardware_p_len,
    nh_webidl_HTML_Window_p_len,
    nh_webidl_HTML_HTMLAreaElement_p_len,
    nh_webidl_HTML_HTMLHeadingElement_p_len,
    nh_webidl_HTML_HTMLObjectElement_p_len,
    nh_webidl_HTML_HTMLTableCaptionElement_p_len,
    nh_webidl_HTML_OffscreenCanvas_p_len,
    nh_webidl_HTML_WindowProxy_p_len,
    nh_webidl_HTML_HTMLAudioElement_p_len,
    nh_webidl_HTML_HTMLHRElement_p_len,
    nh_webidl_HTML_HTMLOListElement_p_len,
    nh_webidl_HTML_HTMLTableCellElement_p_len,
    nh_webidl_HTML_OffscreenCanvasRenderingContext2D_p_len,
    nh_webidl_HTML_WorkerGlobalScope_p_len,
    nh_webidl_HTML_HTMLBaseElement_p_len,
    nh_webidl_HTML_HTMLHtmlElement_p_len,
    nh_webidl_HTML_HTMLOptGroupElement_p_len,
    nh_webidl_HTML_HTMLTableColElement_p_len,
    nh_webidl_HTML_SelectionMode_p_len,
    nh_webidl_HTML_Worker_p_len,
    nh_webidl_HTML_HTMLBodyElement_p_len,
    nh_webidl_HTML_HTMLHyperlinkElementUtils_p_len,
    nh_webidl_HTML_HTMLOptionElement_p_len,
    nh_webidl_HTML_HTMLTableElement_p_len,
    nh_webidl_HTML_SessionStorage_p_len,
    nh_webidl_HTML_WorkerLocation_p_len,
    nh_webidl_HTML_HTMLBRElement_p_len,
    nh_webidl_HTML_HTMLIFrameElement_p_len,
    nh_webidl_HTML_HTMLOrSVGElement_p_len,
    nh_webidl_HTML_HTMLTableRowElement_p_len,
    nh_webidl_HTML_SharedWorkerGlobalScope_p_len,
    nh_webidl_HTML_WorkerNavigator_p_len,
    nh_webidl_HTML_HTMLButtonElement_p_len,
    nh_webidl_HTML_HTMLImageElement_p_len,
    nh_webidl_HTML_HTMLOutputElement_p_len,
    nh_webidl_HTML_HTMLTableSectionElement_p_len,
    nh_webidl_HTML_SharedWorker_p_len,
    nh_webidl_HTML_WorkletGlobalScope_p_len,
    nh_webidl_HTML_HTMLCanvasElement_p_len,
    nh_webidl_HTML_HTMLInputElement_p_len,
    nh_webidl_HTML_HTMLParagraphElement_p_len,
    nh_webidl_HTML_HTMLTemplateElement_p_len,
    nh_webidl_HTML_StorageEvent_p_len,
    nh_webidl_HTML_Worklet_p_len,
    nh_webidl_CSS_CSSGroupingRule_p_len,
    nh_webidl_CSS_CSSMarginRule_p_len,
    nh_webidl_CSS_CSSPageRule_p_len,
    nh_webidl_CSS_CSSRuleList_p_len,
    nh_webidl_CSS_CSSStyleRule_p_len,
    nh_webidl_CSS_Document_p_len,
    nh_webidl_CSS_StyleSheetList_p_len,
    nh_webidl_CSS_CSSImportRule_p_len,
    nh_webidl_CSS_CSSNamespaceRule_p_len,
    nh_webidl_CSS_CSSRule_p_len,
    nh_webidl_CSS_CSSStyleDeclaration_p_len,
    nh_webidl_CSS_CSSStyleSheet_p_len,
    nh_webidl_CSS_CSSCounterStyleRule_p_len,
    nh_webidl_CSS_StyleSheet_p_len,
    nh_webidl_URL_URL_p_len,
    nh_webidl_URL_URLSearchParams_p_len,
};


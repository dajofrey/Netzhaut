#!/bin/bash

# Declare a string array
files=(
    "build/data/idl/DOM/TreeWalker.idl"
    "build/data/idl/DOM/Text.idl"
    "build/data/idl/DOM/StaticRange.idl"
    "build/data/idl/DOM/Slottable.idl"
    "build/data/idl/DOM/ShadowRoot.idl"
    "build/data/idl/DOM/Range.idl"
    "build/data/idl/DOM/ProcessingInstruction.idl"
    "build/data/idl/DOM/ParentNode.idl"
    "build/data/idl/DOM/NonElementParentNode.idl"
    "build/data/idl/DOM/NonDocumentTypeChildNode.idl"
    "build/data/idl/DOM/NodeList.idl"
    "build/data/idl/DOM/NodeIterator.idl"
    "build/data/idl/DOM/NodeFilter.idl"
    "build/data/idl/DOM/Node.idl"
    "build/data/idl/DOM/NamedNodeMap.idl"
    "build/data/idl/DOM/MutationRecord.idl"
    "build/data/idl/DOM/MutationObserver.idl"
    "build/data/idl/DOM/HTMLCollection.idl"
    "build/data/idl/DOM/EventTarget.idl"
    "build/data/idl/DOM/Event.idl"
    "build/data/idl/DOM/Element.idl"
    "build/data/idl/DOM/DocumentType.idl"
    "build/data/idl/DOM/DocumentOrShadowRoot.idl"
    "build/data/idl/DOM/DocumentFragment.idl"
    "build/data/idl/DOM/Document.idl"
    "build/data/idl/DOM/DOMTokenList.idl"
    "build/data/idl/DOM/DOMImplementation.idl"
    "build/data/idl/DOM/Comment.idl"
    "build/data/idl/DOM/ChildNode.idl"
    "build/data/idl/DOM/CharacterData.idl"
    "build/data/idl/DOM/CDataSection.idl"
    "build/data/idl/DOM/Attr.idl"
    "build/data/idl/DOM/AbstractRange.idl"
    "build/data/idl/DOM/AbortSignal.idl"
    "build/data/idl/DOM/AbortController.idl"
    "build/data/idl/HTML/AbstractWorker.idl"
    "build/data/idl/HTML/HTMLDataElement.idl"
    "build/data/idl/HTML/HTMLLabelElement.idl"
    "build/data/idl/HTML/HTMLParamElement.idl"
    "build/data/idl/HTML/HTMLTextAreaElement.idl"
    "build/data/idl/HTML/Storage.idl"
    "build/data/idl/HTML/AudioAndVideoTrackList.idl"
    "build/data/idl/HTML/HTMLDataListElement.idl"
    "build/data/idl/HTML/HTMLLegendElement.idl"
    "build/data/idl/HTML/HTMLPictureElement.idl"
    "build/data/idl/HTML/HTMLTimeElement.idl"
    "build/data/idl/HTML/SubmitEvent.idl"
    "build/data/idl/HTML/CanvasRenderingContext2D.idl"
    "build/data/idl/HTML/HTMLDetailsElement.idl"
    "build/data/idl/HTML/HTMLLIElement.idl"
    "build/data/idl/HTML/HTMLPreElement.idl"
    "build/data/idl/HTML/HTMLTitleElement.idl"
    "build/data/idl/HTML/TextTrackCue.idl"
    "build/data/idl/HTML/CustomElementRegistry.idl"
    "build/data/idl/HTML/HTMLDialogElement.idl"
    "build/data/idl/HTML/HTMLLinkElement.idl"
    "build/data/idl/HTML/HTMLProgressElement.idl"
    "build/data/idl/HTML/HTMLTrackElement.idl"
    "build/data/idl/HTML/TextTrackCueList.idl"
    "build/data/idl/HTML/DedicatedWorkerGlobalScope.idl"
    "build/data/idl/HTML/HTMLDivElement.idl"
    "build/data/idl/HTML/HTMLMapElement.idl"
    "build/data/idl/HTML/HTMLQuoteElement.idl"
    "build/data/idl/HTML/HTMLUListElement.idl"
    "build/data/idl/HTML/TextTrack.idl"
    "build/data/idl/HTML/Document.idl"
    "build/data/idl/HTML/HTMLDListElement.idl"
    "build/data/idl/HTML/HTMLMediaElement.idl"
    "build/data/idl/HTML/HTMLScriptElement.idl"
    "build/data/idl/HTML/HTMLUnknownElement.idl"
    "build/data/idl/HTML/TextTrackList.idl"
    "build/data/idl/HTML/DocumentOrShadowRoot.idl"
    "build/data/idl/HTML/HTMLElement.idl"
    "build/data/idl/HTML/HTMLMediaError.idl"
    "build/data/idl/HTML/HTMLSelectElement.idl"
    "build/data/idl/HTML/HTMLVideoElement.idl"
    "build/data/idl/HTML/TimeRanges.idl"
    "build/data/idl/HTML/DOMStringMap.idl"
    "build/data/idl/HTML/HTMLEmbedElement.idl"
    "build/data/idl/HTML/HTMLMenuElement.idl"
    "build/data/idl/HTML/HTMLSlotElement.idl"
    "build/data/idl/HTML/ImageBitmapRenderingContext.idl"
    "build/data/idl/HTML/TrackEvent.idl"
    "build/data/idl/HTML/ElementInternals.idl"
    "build/data/idl/HTML/HTMLFieldSetElement.idl"
    "build/data/idl/HTML/HTMLMetaElement.idl"
    "build/data/idl/HTML/HTMLSourceElement.idl"
    "build/data/idl/HTML/LocalStorage.idl"
    "build/data/idl/HTML/ValidityState.idl"
    "build/data/idl/HTML/FormDataEvent.idl"
    "build/data/idl/HTML/HTMLFormElement.idl"
    "build/data/idl/HTML/HTMLMeterElement.idl"
    "build/data/idl/HTML/HTMLSpanElement.idl"
    "build/data/idl/HTML/MessageEvent.idl"
    "build/data/idl/HTML/WebSocket.idl"
    "build/data/idl/HTML/HTMLAnchorElement.idl"
    "build/data/idl/HTML/HTMLHeadElement.idl"
    "build/data/idl/HTML/HTMLModElement.idl"
    "build/data/idl/HTML/HTMLStyleElement.idl"
    "build/data/idl/HTML/NavigatorConcurrentHardware.idl"
    "build/data/idl/HTML/Window.idl"
    "build/data/idl/HTML/HTMLAreaElement.idl"
    "build/data/idl/HTML/HTMLHeadingElement.idl"
    "build/data/idl/HTML/HTMLObjectElement.idl"
    "build/data/idl/HTML/HTMLTableCaptionElement.idl"
    "build/data/idl/HTML/OffscreenCanvas.idl"
    "build/data/idl/HTML/WindowProxy.idl"
    "build/data/idl/HTML/HTMLAudioElement.idl"
    "build/data/idl/HTML/HTMLHRElement.idl"
    "build/data/idl/HTML/HTMLOListElement.idl"
    "build/data/idl/HTML/HTMLTableCellElement.idl"
    "build/data/idl/HTML/OffscreenCanvasRenderingContext2D.idl"
    "build/data/idl/HTML/WorkerGlobalScope.idl"
    "build/data/idl/HTML/HTMLBaseElement.idl"
    "build/data/idl/HTML/HTMLHtmlElement.idl"
    "build/data/idl/HTML/HTMLOptGroupElement.idl"
    "build/data/idl/HTML/HTMLTableColElement.idl"
    "build/data/idl/HTML/SelectionMode.idl"
    "build/data/idl/HTML/Worker.idl"
    "build/data/idl/HTML/HTMLBodyElement.idl"
    "build/data/idl/HTML/HTMLHyperlinkElementUtils.idl"
    "build/data/idl/HTML/HTMLOptionElement.idl"
    "build/data/idl/HTML/HTMLTableElement.idl"
    "build/data/idl/HTML/SessionStorage.idl"
    "build/data/idl/HTML/WorkerLocation.idl"
    "build/data/idl/HTML/HTMLBRElement.idl"
    "build/data/idl/HTML/HTMLIFrameElement.idl"
    "build/data/idl/HTML/HTMLOrSVGElement.idl"
    "build/data/idl/HTML/HTMLTableRowElement.idl"
    "build/data/idl/HTML/SharedWorkerGlobalScope.idl"
    "build/data/idl/HTML/WorkerNavigator.idl"
    "build/data/idl/HTML/HTMLButtonElement.idl"
    "build/data/idl/HTML/HTMLImageElement.idl"
    "build/data/idl/HTML/HTMLOutputElement.idl"
    "build/data/idl/HTML/HTMLTableSectionElement.idl"
    "build/data/idl/HTML/SharedWorker.idl"
    "build/data/idl/HTML/WorkletGlobalScope.idl"
    "build/data/idl/HTML/HTMLCanvasElement.idl"
    "build/data/idl/HTML/HTMLInputElement.idl"
    "build/data/idl/HTML/HTMLParagraphElement.idl"
    "build/data/idl/HTML/HTMLTemplateElement.idl"
    "build/data/idl/HTML/StorageEvent.idl"
    "build/data/idl/HTML/Worklet.idl"
    "build/data/idl/CSS/CSSGroupingRule.idl" 
    "build/data/idl/CSS/CSSMarginRule.idl"
    "build/data/idl/CSS/CSSPageRule.idl"
    "build/data/idl/CSS/CSSRuleList.idl"         
    "build/data/idl/CSS/CSSStyleRule.idl"   
    "build/data/idl/CSS/Document.idl"
    "build/data/idl/CSS/StyleSheetList.idl"
    "build/data/idl/CSS/CSSImportRule.idl"   
    "build/data/idl/CSS/CSSNamespaceRule.idl"
    "build/data/idl/CSS/CSSRule.idl"
    "build/data/idl/CSS/CSSStyleDeclaration.idl"
    "build/data/idl/CSS/CSSStyleSheet.idl"
    "build/data/idl/CSS/CSSCounterStyleRule.idl"
    "build/data/idl/CSS/StyleSheet.idl"
    "build/data/idl/URL/URL.idl"
    "build/data/idl/URL/URLSearchParams.idl")

# Iterate through each string in the array
for f in "${files[@]}"; do
    filename=${f#*idl/}
    new_string=$(echo "$f" | sed 's|.*/\([^/]*\)/\([^/]*\)\.idl|\1_\2|')
    ./external/xxd/xxd -i $f src/lib/nh-webidl/Specifications/$filename.inc nh_webidl_${new_string}_p
done

# License notice
LICENSE_NOTICE="\
// LICENSE NOTICE ==================================================================================\n\n\
/**\n\
 * Netzhaut - Web Browser Engine\n\
 * Copyright (C) 2022  Dajo Frey\n\
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.\n\
 *\n\
 * This file was generated.\n\
 */\n\n"

# Output file
OUTPUT_FILE="src/lib/nh-webidl/Runtime/Builtin.c"

# Function to extract the filename from the path
get_filename() {
    path=$1
    echo "${path#build/data/idl/}"
}

# Generate the output file
generate_output() {
    # Write license notice
    echo -e "$LICENSE_NOTICE" > "$OUTPUT_FILE"

    # Write includes
    echo -e "// INCLUDE =========================================================================================\n" >> "$OUTPUT_FILE"
    echo -e "#include \"Builtin.h\"\n" >> "$OUTPUT_FILE"

    # Write file includes
    for file in "${files[@]}"; do
        echo "#include \"../Specifications/$(get_filename "$file").inc\"" >> "$OUTPUT_FILE"
    done

    echo -e "\n// FRAGMENT_NAMES ==================================================================================\n" >> "$OUTPUT_FILE"
    echo -e "NH_BYTE *NH_WEBIDL_FRAGMENT_NAMES_PP[] = {" >> "$OUTPUT_FILE"
    for file in "${files[@]}"; do
        new_name=$(echo "$file" | sed 's|build/data/idl/||; s|/|_|g; s/\.idl$//')
        echo "    \"$new_name\"," >> "$OUTPUT_FILE"
    done
    echo -e "};\n" >> "$OUTPUT_FILE"
    echo -e "size_t NH_WEBIDL_FRAGMENT_NAMES_PP_COUNT = sizeof(NH_WEBIDL_FRAGMENT_NAMES_PP) / sizeof(NH_WEBIDL_FRAGMENT_NAMES_PP[0]);\n" >> "$OUTPUT_FILE"

    echo -e "// FRAGMENTS =======================================================================================\n" >> "$OUTPUT_FILE"
    echo -e "NH_UNSIGNED_BYTE *NH_WEBIDL_FRAGMENTS_PP[] = {" >> "$OUTPUT_FILE"
    for file in "${files[@]}"; do
        new_name=$(echo "$file" | sed 's|build/data/idl/||; s|/|_|g; s/\.idl$//')
        echo "    nh_webidl_${new_name}_p," >> "$OUTPUT_FILE"
    done
    echo -e "};\n" >> "$OUTPUT_FILE"
    echo -e "size_t NH_WEBIDL_FRAGMENTS_PP_COUNT = sizeof(NH_WEBIDL_FRAGMENTS_PP) / sizeof(NH_WEBIDL_FRAGMENTS_PP[0]);\n" >> "$OUTPUT_FILE"

    echo -e "// FRAGMENT LENGTHS ================================================================================\n" >> "$OUTPUT_FILE"
    echo -e "const unsigned int NH_WEBIDL_FRAGMENT_LENGTHS_P[] = {" >> "$OUTPUT_FILE"
    for file in "${files[@]}"; do
        new_name=$(echo "$file" | sed 's|build/data/idl/||; s|/|_|g; s/\.idl$//')
        echo "    nh_webidl_${new_name}_p_len," >> "$OUTPUT_FILE"
    done
    echo -e "};\n" >> "$OUTPUT_FILE"
}

# Call the function to generate the output
generate_output

# Print success message
echo "Output file '$OUTPUT_FILE' generated successfully."

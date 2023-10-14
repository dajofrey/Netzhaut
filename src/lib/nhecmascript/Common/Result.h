#ifndef NH_ECMASCRIPT_RESULT_H
#define NH_ECMASCRIPT_RESULT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#endif

/** @{ */ 

    #define NH_ECMASCRIPT_DOM_EXCEPTION_HIERARCHY_REQUEST_ERROR "HierarchyRequestError"
    #define NH_ECMASCRIPT_DOM_EXCEPTION_HIERARCHY_REQUEST_ERROR_MESSAGE "The operation would yield an incorrect node tree."

    #define NH_ECMASCRIPT_DOM_EXCEPTION_WRONG_DOCUMENT_ERROR "WrongDocumentError"
    #define NH_ECMASCRIPT_DOM_EXCEPTION_WRONG_DOCUMENT_ERROR_MESSAGE "The object is in the wrong document."

    #define NH_ECMASCRIPT_DOM_EXCEPTION_INVALID_CHARACTER_ERROR "InvalidCharacterError"
    #define NH_ECMASCRIPT_DOM_EXCEPTION_INVALID_CHARACTER_ERROR_MESSAGE "The string contains invalid characters."

    #define NH_ECMASCRIPT_DOM_EXCEPTION_NO_MODIFICATION_ALLOWED_ERROR "NoModificationAllowedError"
    #define NH_ECMASCRIPT_DOM_EXCEPTION_NO_MODIFICATION_ALLOWED_ERROR_MESSAGE "The object can not be modified."

    #define NH_ECMASCRIPT_DOM_EXCEPTION_NOT_FOUND_ERROR "NotFoundError"
    #define NH_ECMASCRIPT_DOM_EXCEPTION_NOT_FOUND_ERROR_MESSAGE "The object can not be found here."

    #define NH_ECMASCRIPT_DOM_EXCEPTION_NOT_SUPPORTED_ERROR "NotSupportedError"
    #define NH_ECMASCRIPT_DOM_EXCEPTION_NOT_SUPPORTED_ERROR_MESSAGE "The operation is not supported."

    #define NH_ECMASCRIPT_DOM_EXCEPTION_IN_USE_ATTRIBUTE_ERROR "InUseAttributeError"
    #define NH_ECMASCRIPT_DOM_EXCEPTION_IN_USE_ATTRIBUTE_ERROR_MESSAGE "The attribute is in use."

    #define NH_ECMASCRIPT_DOM_EXCEPTION_INVALID_STATE_ERROR "InvalidStateError"
    #define NH_ECMASCRIPT_DOM_EXCEPTION_INVALID_STATE_ERROR_MESSAGE "The object is in an invalid state."

    #define NH_ECMASCRIPT_DOM_EXCEPTION_SYNTAX_ERROR "SyntaxError"
    #define NH_ECMASCRIPT_DOM_EXCEPTION_SYNTAX_ERROR_MESSAGE "The string did not match the expected pattern."

    #define NH_ECMASCRIPT_DOM_EXCEPTION_INVALID_MODIFICATION_ERROR "InvalidModificationError"
    #define NH_ECMASCRIPT_DOM_EXCEPTION_INVALID_MODIFICATION_ERROR_MESSAGE "The object can not be modified in this way."

    #define NH_ECMASCRIPT_DOM_EXCEPTION_NAMESPACE_ERROR "NamespaceError"
    #define NH_ECMASCRIPT_DOM_EXCEPTION_NAMESPACE_ERROR_MESSAGE "The operation is not allowed by Namespaces in XML."

    #define NH_ECMASCRIPT_DOM_EXCEPTION_SECURITY_ERROR "SecurityError"
    #define NH_ECMASCRIPT_DOM_EXCEPTION_SECURITY_ERROR_MESSAGE "The operation is insecure."

    #define NH_ECMASCRIPT_DOM_EXCEPTION_NETWORK_ERROR "NetworkError"
    #define NH_ECMASCRIPT_DOM_EXCEPTION_NETWORK_ERROR_MESSAGE "A network error occurred."

    #define NH_ECMASCRIPT_DOM_EXCEPTION_ABORT_ERROR "AbortError"
    #define NH_ECMASCRIPT_DOM_EXCEPTION_ABORT_ERROR_MESSAGE "The operation was aborted."

    #define NH_ECMASCRIPT_DOM_EXCEPTION_URL_MISMATCH_ERROR "URLMismatchError"
    #define NH_ECMASCRIPT_DOM_EXCEPTION_URL_MISMATCH_ERROR_MESSAGE "The given URL does not match another URL."

    #define NH_ECMASCRIPT_DOM_EXCEPTION_QUOTA_EXCEEDED_ERROR "QuotaExceededError"
    #define NH_ECMASCRIPT_DOM_EXCEPTION_QUOTA_EXCEEDED_ERROR_MESSAGE "The quota has been exceeded."

    #define NH_ECMASCRIPT_DOM_EXCEPTION_TIMEOUT_ERROR "TimeoutError"
    #define NH_ECMASCRIPT_DOM_EXCEPTION_TIMEOUT_ERROR_MESSAGE "The operation timed out."

    #define NH_ECMASCRIPT_DOM_EXCEPTION_INVALID_NODE_TYPE_ERROR "InvalidNodeTypeError"
    #define NH_ECMASCRIPT_DOM_EXCEPTION_INVALID_NODE_TYPE_ERROR_MESSAGE "The supplied node is incorrect or has an incorrect ancestor for this operation."

    #define NH_ECMASCRIPT_DOM_EXCEPTION_DATA_CLONE_ERROR "DataCloneError"
    #define NH_ECMASCRIPT_DOM_EXCEPTION_DATA_CLONE_ERROR_MESSAGE "The object can not be cloned."

    #define NH_ECMASCRIPT_DOM_EXCEPTION_ENCODING_ERROR "EncodingError"
    #define NH_ECMASCRIPT_DOM_EXCEPTION_ENCODING_ERROR_MESSAGE "The encoding operation (either encoded or decoding) failed."

    #define NH_ECMASCRIPT_DOM_EXCEPTION_NOT_READABLE_ERROR "NotReadableError"
    #define NH_ECMASCRIPT_DOM_EXCEPTION_NOT_READABLE_ERROR_MESSAGE "The I/O read operation failed."

    #define NH_ECMASCRIPT_DOM_EXCEPTION_UNKNOWN_ERROR "UnknownError"
    #define NH_ECMASCRIPT_DOM_EXCEPTION_UNKNOWN_ERROR_MESSAGE "The operation failed for an unknown transient reason (e.g. out of memory)."

    #define NH_ECMASCRIPT_DOM_EXCEPTION_CONSTRAINT_ERROR "ConstraintError"
    #define NH_ECMASCRIPT_DOM_EXCEPTION_CONSTRAINT_ERROR_MESSAGE "A mutation operation in a transaction failed because a constraint was not satisfied."

    #define NH_ECMASCRIPT_DOM_EXCEPTION_DATA_ERROR "DataError"
    #define NH_ECMASCRIPT_DOM_EXCEPTION_DATA_ERROR_MESSAGE "Provided data is inadequate."

    #define NH_ECMASCRIPT_DOM_EXCEPTION_TRANSACTION_INACTIVE_ERROR "TransactionInactiveError"
    #define NH_ECMASCRIPT_DOM_EXCEPTION_TRANSACTION_INACTIVE_ERROR_MESSAGE "A request was placed against a transaction which is currently not active, or which is finished."

    #define NH_ECMASCRIPT_DOM_EXCEPTION_READ_ONLY_ERROR "ReadOnlyError"
    #define NH_ECMASCRIPT_DOM_EXCEPTION_READ_ONLY_ERROR_MESSAGE "The mutating operation was attempted in a readonly transaction."

    #define NH_ECMASCRIPT_DOM_EXCEPTION_VERSION_ERROR "VersionError"
    #define NH_ECMASCRIPT_DOM_EXCEPTION_VERSION_ERROR_MESSAGE "An attempt was made to open a database using a lower version than the existing version."

    #define NH_ECMASCRIPT_DOM_EXCEPTION_OPERATION_ERROR "OperationError"
    #define NH_ECMASCRIPT_DOM_EXCEPTION_OPERATION_ERROR_MESSAGE "The operation failed for an operation-specific reason."

    #define NH_ECMASCRIPT_DOM_EXCEPTION_NOT_ALLOWED_ERROR "NotAllowedError"
    #define NH_ECMASCRIPT_DOM_EXCEPTION_NOT_ALLOWED_ERROR_MESSAGE "The request is not allowed by the user agent or the platform in the current context, possibly because the user denied permission."

/** @} */

/** @{ */

    extern const char *NH_ECMASCRIPT_RESULTS_PP[];
    extern unsigned int NH_ECMASCRIPT_RESULTS_PP_COUNT;

/** @} */

#endif

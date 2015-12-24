package hu.sandorfehervari.analogdisplayreader;

import java.util.HashMap;
import java.util.Map;

/**
 * Created by fehervarisandor on 2015. 10. 18..
 */
public enum ErrorCause {
    INVALID_RESULT,
    ERROR_INVALID_INPUT_IMAGE,
    ERROR_EMPTY_IMAGE,
    ERROR_FAILED_TO_DETECT_NUMBER_PLATE,
    ERROR_FAILED_TO_DETECT_INDICATOR,
    ERROR_FAILED_TO_DETECT_NUMBER_FIELDS,
    ERROR_FAILED_TO_DETECT_NUMBER_FIELDS_DIGIT;

    private static final Map<String, ErrorCause> map = new HashMap<>();

    static {
        map.put("-1", INVALID_RESULT);
        map.put("-2", ERROR_INVALID_INPUT_IMAGE);
        map.put("-100", ERROR_EMPTY_IMAGE);
        map.put("-200", ERROR_FAILED_TO_DETECT_NUMBER_PLATE);
        map.put("-300", ERROR_FAILED_TO_DETECT_INDICATOR);
        map.put("-400", ERROR_FAILED_TO_DETECT_NUMBER_FIELDS);
        map.put("-500", ERROR_FAILED_TO_DETECT_NUMBER_FIELDS_DIGIT);
    }

    public static ErrorCause getErrorCauseByErrorCode(String errorCode) {
        return map.get(errorCode);
    }

}

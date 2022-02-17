#include "autocomplete.h"
#include "predictions.h"

char* custom_input(Tree* rules, char* title, COLOR_TYPE title_color, COLOR_TYPE predict_color,
    COLOR_TYPE main_color, char* optional_brackets) {
    short buff_len = 0;
    short buff_cap = terminal_width();
    char* buff = (char*)calloc((size_t)buff_cap, sizeof(char));
    if (buff == NULL) {
        fprintf(stderr, "[ERROR] Couldn't allocate memory for buffer\n");
        exit(1);
    }


    int offset = 0;


    int hint_num = 0;


    int title_len = (short)strlen(title);

    while (1) {

        clear_line();
        color_print(title, title_color);
        printf(title_len != 0 ? " " : "");


        short space_offset = 0;
        while ((buff_len - space_offset) != 0 && buff[buff_len - space_offset - 1] != ' ') {
            space_offset += 1;
        }


        color_print(buff, main_color);

        Predictions* pred = predictions_create(rules, buff, optional_brackets);
        if (pred->type != FAILURE) {
            char* prediction = (char*)vector_get(pred->tokens, hint_num % pred->tokens->length);
            if (pred->type == PROBABLY) {
                color_print(title, predict_color);
            }

            color_print(prediction + (pred->type == EXACTLY) * space_offset, predict_color);
        }


        short x = (short)(buff_len + title_len + (title_len != 0) + 1 - offset);
        set_cursor_x(x);


        int ch = _getch();


        if (is_ignore_key(ch)) {
            continue;
        }


        else if (ch == ENTER) {

            predictions_free(pred);
            break;
        }


#if defined(OS_WINDOWS)
        else if (ch == CTRL_C) {
            predictions_free(pred);
            tree_free(rules);
            free(buff);
            exit(0);
        }
#endif


        else if (ch == BACKSPACE) {
            if (buff_len != 0 && buff_len - offset >= 1) {

                for (unsigned i = (unsigned int)(buff_len - offset - 1); i < buff_cap - 1; i++) {
                    buff[i] = buff[i + 1];
                }
                buff_len -= 1;
            }
        }


        else if (ch == TAB) {
            if (pred->type != FAILURE) {
                char* prediction = (char*)vector_get(pred->tokens, hint_num % pred->tokens->length);
                unsigned predict_len = (unsigned int)strlen(prediction);


                if (!contain_chars(prediction, optional_brackets)) {


                    if (buff_len + predict_len - space_offset >= buff_cap) {
                        fprintf(stderr, "\n[ERROR] Input string more then terminal width\n");
                        exit(1);
                    }


                    for (unsigned i = 0; i < predict_len; i++) {
                        buff[buff_len - space_offset] = prediction[i];
                        buff_len++;
                    }


                    buff_len = buff_len - space_offset;


                    buff[buff_len++] = ' ';
                    buff[buff_len] = '\0';
                }
            }
        }


        else if (
            ch == SPECIAL_SEQ_1
#if defined(OS_WINDOWS)
            || ch
#endif
            == SPECIAL_SEQ_2
            ) {
            switch (_getch()) {
            case LEFT:

                offset = (offset < buff_len) ? (offset + 1) : buff_len;
                break;
            case RIGHT:

                offset = (offset > 0) ? offset - 1 : 0;
                break;
            case UP:

                hint_num = hint_num + 1;
                clear_line();
                break;
            case DOWN:

                hint_num = hint_num - 1;
                clear_line();
                break;
            case DEL:

#if defined(OS_UNIX)
                if (_getch() == DEL_AFTER)
#endif
                {
                    if (buff_len != 0 && offset != 0) {
                        for (unsigned i = (unsigned int)(buff_len - offset); i < buff_cap - 1; i++) {
                            buff[i] = buff[i + 1];
                        }
                        buff_len -= 1;
                        offset -= 1;
                    }
                }
                break;
            default:
                break;
            }
        }

        else {
            hint_num = ch == SPACE ? 0 : hint_num;
            buff[buff_len++] = (char)ch;
        }

        predictions_free(pred);
    }

    return buff;
}

char* input(Tree* rules) {
    return custom_input(rules, "", DEFAULT_TITLE_COLOR, DEFAULT_PREDICT_COLOR,
        DEFAULT_MAIN_COLOR, "");
}

int is_ignore_key(int ch) {
    int ignore_keys[] =
#if defined(OS_WINDOWS)
    { 1, 2, 19, 24, 26 };
#endif

    unsigned len = sizeof(ignore_keys) / sizeof(int);

    for (unsigned i = 0; i < len; i++) {
        if (ch == ignore_keys[i]) {
            return 1;
        }
    }

    return 0;
}
typedef enum {
	YESNO_QUESTION_ANSWER_NONE,
	YESNO_QUESTION_ANSWER_YES,
	YESNO_QUESTION_ANSWER_NO,
} yesno_question_answer;
void show_yesno_question(const char *question_text, void hidden_callback(yesno_question_answer));
void hide_yesno_question(void);

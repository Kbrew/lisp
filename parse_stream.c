
int peek(ParseStream *in);
void skip_whitespace(ParseStream *in);

ParseStream *parse_stream_new(FILE* file, SourceLocation loc){
	ParseStream *val;

	val = malloc(sizeof(ParseStream));
	if (val == NULL) {
		fprintf(stderr, "out of memory\n");
		exit(1);
	}
	
	val->file = file;
	val->loc = loc;
	
	return val;
}

typedef struct ParseStream {
	FILE *file;
	SourceLocation loc;
} ParseStream;
int peek(ParseStream *in);
void skip_whitespace(ParseStream *in);

ParseStream *parse_stream_new(FILE* file, SourceLocation loc);

typedef struct ParseStream ParseStream;
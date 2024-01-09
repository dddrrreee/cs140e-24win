static char line[1024*8];
char *getline(void) {
    if(fgets(line, sizeof line, stdin))
        return line;
    return 0;
}

static void trim(char *buf) {
    unsigned n = strlen(buf);
    char *end = &buf[n-1];

    assert(*end == '\n');
    end--;
    if(*end == '\')
        end--;
    while(end > buf && isspace(*end))
        end--;
    end[1] = 0;
}

static void emit(char *buf) {
    trim(buf);
    printf("%80s\\\n", buf);
}

static void process_def(char *buf) {
    char *l;

    emit(line);
    while(getline()) {
    }
}


int main(void) { 
    int c;
    char buf[1024*8];

    char *l;

    while(getline()) {
    while(!is_def())
        printf("%s", line);

    while((l = getline())) {
        char *def = "#define";
        if(strncmp(def, l, strlen(l)) == 0)
            process_def(buf);
    }
}

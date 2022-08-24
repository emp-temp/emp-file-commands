#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void cat(char *filename, int start_num, int end_num);
void print_usage();
void print_error(char *text);
void encoding(char *filename);
void replacement(char *filename, int change_line_num);
void deleate(char *filename, int delate_line_num);
void insert(char *filename, int insert_num);
void search(char *filename, char *search_word);
void write(char *file, char *write_word, char *option);
void dictionary(char *word);
int get_file_line(char *filename);
int is_included(int start_num, int end_num, int limit_num);
int is_greater(int start_num, int end_num);
size_t split(char *s, const char *separator, char **result, size_t result_size);

#define N 256

#define SIZE_OF_ARRAY(array) (sizeof(array) / sizeof(array[0]))

int main(int argc, char *argv[]) {
  char option = ' ';
  for (int i = 1; i < argc; i++) {
    if (*argv[i] == '-') {
      option = *(argv[i] + 1);
      switch (option) {
      case 'c':
        if (argc == 4) {
          cat(argv[i + 1], atoi(argv[i + 2]), 0);
        } else if (argc == 5) {
          cat(argv[i + 1], atoi(argv[i + 2]), atoi(argv[i + 3]));
        } else if (argc == 3) {
          cat(argv[i + 1], 0, 0);
        }
        break;

      case 'e':
        encoding(argv[i + 1]);
        break;

      case 'r':
        if (argc != 4) {
          printf("argc: %d\n", argc);
          print_usage();
          print_error("\n\nNOT ENOUGH ARGUMENTS");
        }
        replacement(argv[i + 1], atoi(argv[i + 2]));
        break;

      case 'i':
        if (argc != 4) {
          printf("argc: %d\n", argc);
          print_usage();
          print_error("\n\nNOT ENOUGH ARGUMENTS");
        }
        insert(argv[i + 1], atoi(argv[i + 2]));
        break;

      case 'w':
        if (argc != 4) {
          printf("argc: %d\n", argc);
          print_usage();
          print_error("\n\nNOT ENOUGH ARGUMENTS");
        }
        write(argv[i + 1], argv[i + 2], argv[i]);
        break;

      case 'd':
        if (argc != 4) {
          printf("argc: %d\n", argc);
          print_usage();
          print_error("\n\nNOT ENOUGH ARGUMENTS");
        }
        deleate(argv[i + 1], atoi(argv[i + 2]));
        break;

      case 's':
        if (argc != 4) {
          printf("argc: %d\n", argc);
          print_usage();
          print_error("\n\nNOT ENOUGH ARGUMENTS");
        }
        search(argv[i + 1], argv[i + 2]);
        break;

      case 'h':
        print_usage();
        break;

      default:
        print_usage();
        break;
      }
    }
    if ((strcmp(argv[i], "dic") == 0) && i == 1) {
      if (argc != 3) {
        printf("argc: %d\n", argc);
        print_usage();
        print_error("\n\nNOT ENOUGH ARGUMENTS");
      }
      dictionary(argv[argc - 1]);
    } 
  }
  return 0;
}

//可変長引数使いたかったけど断念
void cat(char *filename, int start_num, int end_num) {
  FILE *fp;
  int line_num = 1;
  int limit_num = get_file_line(filename);
  int chr;

  fp = fopen(filename, "r");

  if (fp == NULL) {
    char *text;
    sprintf(text, "%s file not open!\n", filename);
    print_error(text);
  }

  printf("\n%s file opened!\n\n", filename);

  if (start_num == 0 && end_num == 0) {
    printf("  1|");
    while ((chr = fgetc(fp)) != EOF) {
      putchar(chr);
      if (chr == '\n' && (line_num != limit_num)) {
        line_num++;
        printf("%3d|", line_num);
      }
    }
  } else {
    if (!(is_included(start_num, end_num, limit_num))) {
      char *text;
      sprintf(text, "[ERROR] VALUE IS NOT INCLUDED (start: 1, end: %d)",
              limit_num);
      print_error(text);
    }
    if (end_num == 0) {
      for (int i = 0; i < start_num; i++) {
        char str[N];
        fgets(str, N, fp);
        if (i == (start_num - 1)) {
          printf("%3d|%s", i + 1, str);
        }
      }
    } else {
      if (!(is_greater(start_num, end_num))) {
        print_usage();
        exit(1);
      }
      for (int i = 0; i < limit_num; i++) {
        char str[N];
        fgets(str, N, fp);
        if (i >= (start_num - 1) && i <= (end_num - 1)) {
          printf("%3d|%s", i + 1, str);
        }
      }
    }
  }

  fclose(fp);
}

int get_file_line(char *filename) {

  FILE *fp;
  char buf[N];
  int line = 0;

  fp = fopen(filename, "r");

  if (fp == NULL) {
    char *text;
    sprintf(text, "%s file not open!\n", filename);
    print_error(text);
  }

  while (fgets(buf, N, fp) != NULL) {
    line++;
  }

  return line;
}

int is_greater(int start_num, int end_num) {
  if (start_num < end_num) {
    return 1;
  } else {
    return 0;
  }
}

int is_included(int start_num, int end_num, int limit_num) {
  if (0 < start_num && start_num <= limit_num && end_num <= limit_num) {
    return 1;
  } else {
    return 0;
  }
}

void print_usage() {
  FILE *fp;
  char str[N];
  fp = fopen("/Users/empty/my_commands/ecm/usage.txt", "r");
  if (fp == NULL) {
    print_error("usage.txt file not open!");
  }
  while (fgets(str, N, fp) != NULL) {
    printf("%s", str);
  }

  fclose(fp);
}

void print_error(char *text) {
  printf("\n%s\n", text);
  exit(1);
}

void encoding(char *filename) {
  FILE *fp;
  int chr;

  fp = fopen(filename, "r");

  if (fp == NULL) {
    char *text;
    sprintf(text, "%s file not open!", filename);
    print_error(text);
  }

  while ((chr = fgetc(fp)) != EOF) {
    if (chr == '\n') {
      printf("\\n");
    } else if (chr == '"') {
      printf("\"");
    } else {
      putchar(chr);
    }
  }

  fclose(fp);
}

void replacement(char *filename, int change_line_num) {
  FILE *fp;
  char str[N][10000];
  char change_word[10000];
  int line_num = 0;

  fp = fopen(filename, "r");
  if (fp == NULL) {
    char *text;
    sprintf(text, "%s file not opened!", filename);
    print_error(text);
  }

  printf("\n%s file opened!\n\n", filename);

  while (fgets(str[line_num], N, fp) != NULL) {
    line_num++;
  }

  printf(">>>");

  scanf("%[^\n]%*c", change_word);
  strcpy(str[change_line_num - 1], change_word);
  strcat(str[change_line_num - 1], "\n");

  fclose(fp);

  fp = fopen(filename, "w");

  if (fp == NULL) {
    char *text;
    sprintf(text, "%s file not opened!", filename);
    print_error(text);
  }

  for (int i = 0; i <= line_num; i++) {
    fputs(str[i], fp);
  }

  fclose(fp);
}

void deleate(char *filename, int delate_line_num) {
  FILE *fp;
  char str[N][10000];
  char change_word[10000];
  int line_num = 0;

  fp = fopen(filename, "r");
  if (fp == NULL) {
    char *text;
    sprintf(text, "%s file not opened!", filename);
    print_error(text);
  }

  printf("\n%s file opened!\n\n", filename);

  while (fgets(str[line_num], N, fp) != NULL) {
    line_num++;
  }

  for (int i = delate_line_num - 1; i < line_num; i++) {
    strcpy(str[i], str[i + 1]);
  }

  fclose(fp);

  fp = fopen(filename, "w");

  if (fp == NULL) {
    char *text;
    sprintf(text, "%s file not opened!", filename);
    print_error(text);
  }

  for (int i = 0; i < line_num; i++) {
    fputs(str[i], fp);
  }

  fclose(fp);

  printf("[SUCCESSED!!]");
}

void insert(char *filename, int insert_num) {

  if (insert_num < 0) {
    print_error("ILLEGAL VALUE");
  }

  FILE *fp;
  char str[N][10000];
  char insert_word[10000];
  int line_num = 0;

  fp = fopen(filename, "r");
  if (fp == NULL) {
    char *text;
    sprintf(text, "%s file not opened!", filename);
    print_error(text);
  }

  printf("\n%s file opened!\n\n", filename);

  while (fgets(str[line_num], N, fp) != NULL) {
    line_num++;
  }

  printf(">>>");
  for (int i = line_num; i > insert_num - 1; i--) {
    strcpy(str[i + 1], str[i]);
  }

  scanf("%[^\n]%*c", insert_word);
  strcpy(str[insert_num], insert_word);
  strcat(str[insert_num], "\n");

  fclose(fp);

  fp = fopen(filename, "w");

  if (fp == NULL) {
    char *text;
    sprintf(text, "%s file not opened!", filename);
    print_error(text);
  }

  for (int i = 0; i < line_num + 1; i++) {
    fputs(str[i], fp);
  }

  fclose(fp);

  printf("[SUCCESSED!!]");
}

size_t split(char *s, const char *separator, char **result,
             size_t result_size) {
  assert(s != NULL);
  assert(separator != NULL);
  assert(result != NULL);
  assert(result_size > 0);

  size_t i = 0;

  char *p = strtok(s, separator);
  while (p != NULL) {
    assert(i < result_size);
    result[i] = p;
    ++i;

    p = strtok(NULL, separator);
  }

  return i;
}

void search(char *filename, char *search_word) {

  FILE *fp;
  char str[N][10000];
  int line_num = 0;
  char *result[10000];
  size_t result_size;

  fp = fopen(filename, "r");
  if (fp == NULL) {
    char *text;
    sprintf(text, "%s file not opened!", filename);
    print_error(text);
  }

  printf("\n%s file opened!\n\n", filename);

  while (fgets(str[line_num], N, fp) != NULL) {
    line_num++;
  }

  for (int i = 0; i < line_num; i++) {
    if (strstr(str[i], search_word) != NULL) {
      printf("\n%d|%s\n", i + 1, str[i]);
    }
  }
}

void write(char *filename, char *write_word, char *option) {

  char str[N][10000];
  int num_of_line;

  if (strcmp(option, "-wm") == 0) {
    num_of_line = atoi(write_word);
    for (int i = 0; i < num_of_line; i++) {
      printf(">>>");
      scanf("%[^\n]%*c", str[i]);
    }
  } else {
    num_of_line = 1;
    strcpy(str[0], write_word);
  }

  FILE *fp;

  fp = fopen(filename, "w");

  if (fp == NULL) {
    char *text;
    sprintf(text, "%s file not opened!", filename);
    print_error(text);
  }

  for (int i = 0; i < num_of_line; i++) {
    fprintf(fp, "%s\n", str[i]);
  }

  fclose(fp);
}

void dictionary(char *word) {
  FILE *fp;
  char fname[] = "/Users/empty/my_commands/ecm/ejdict-hand-utf8.txt";
  char str[N];
  char *result[8];
  size_t result_size;
  char *line;

  fp = fopen(fname, "r");
  if (fp == NULL) {
    print_error("dictionary file not cat't opened");
  }
  while (fgets(str, N, fp) != NULL) {
    line = strstr(str, word);
    if (line == NULL) {
      continue;
    }
    result_size = split(str, "	", result, SIZE_OF_ARRAY(result));
    for (size_t i = 0; i < result_size; ++i) {
      if (strcmp(word, result[i]) != 0) {
        continue;
      }
      printf("%s    %s\n", result[0], result[1]);
    }
  }

  fclose(fp);

}

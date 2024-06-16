#include <stdio.h>
#include <stdlib.h>


char getHexDigit(FILE *file) {

    char c = ' ';
    while (c != EOF && c != '#') 
        c = fgetc(file);
    if (c != EOF) 
        c = fgetc(file);
    
    
    return c;
}

char output(FILE *file, int w, int h) {
//     w = (w / 2) + 1;
    char c;
    
    int digit = 1;

    for (int y = 0; y < h; y++) 
    {
        printf("\n\t");
        for (int x = 0; x < w; x++) 
        {
            c = getHexDigit(file);
            if (c != EOF) 
            {
                if (digit == 1) 
                {
                    printf("0x%c", c);
                    digit++;
                }
                else if (digit == 2) 
                {
                    printf("%c, ", c);
                    digit = 1;
                }
                else
                    printf("%c", c);
            }
        }
    }   
}


int main (int argc, char* argv[]) {

    int width = 32;
    int height = 32;
    char *inputfile;
    char *name;
    
    if (argc > 0)
        inputfile = argv[1];
    else
        inputfile = "./output-onlinepngtools.txt";
        
    if (argc >= 3) {
        width = atoi(argv[2]);
        height = atoi(argv[3]);
    }
    if (argc >= 4)
        name = argv[4];
    else 
        name = "test_array";
        
    printf("///////////////////////////////////////////////////////////////////////////////\n\n");
    printf("// %d x %d 4BIT bitmap\nconst unsigned char %s [] PROGMEM = {\n", width, height, name);
    
    FILE *file = fopen(inputfile, "r");
    if (file == NULL) {
        printf("ERROR: Cannot open file %s", inputfile);
        return 0;
    }
    output(file, width, height);
    
    printf("\n};\n///////////////////////////////////////////////////////////////////////////////\n");
    
    return 0;
}

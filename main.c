//#include <stdio.h>
//#include <stdlib.h>

#include "render.c"

bool start();
void main_loop();


char * song = "seaShanty";

Note notes[400];
int amountNotes = 0;

int main()
{

    printf("Hello world!\n");


    load_map("song/seaShanty.osu");




    start();


    main_loop();
    //getchar();
    return 0;
}


int string_int (char str [7])
{
    int output = 0;
    for(int i = 0; i < 7; i++)
    {
        if(str[i] < '0' || str[i] > '9') continue;
        output = output*10 + (str[i] - '0');
    }

    return output;
}

void main_loop ()
{
    float time = -5000;
    int currentNote = 0;
    float errorTimeout = 0;
    while(frame_update())
    {


        //update camera
        time += delta * 1000;

        if(errorTimeout > 0) errorTimeout -= delta * 8;
        if(errorTimeout < 0) errorTimeout = 0;


        //printf("jump_moment - millitotal = %i\n", millitotal - jump_moment);
        for(int i = currentNote; i < currentNote+8; i++)
        {
            float margin = 200;

            if((notes[i].longTime == 0 && notes[i].timing - margin < time) || (notes[i].longTime != 0 && notes[i].longTime - margin < time)) {
                //errorTimeout = 2;
                if(keys[0] && notes[i].position == 64) { currentNote++; continue; }
                if(keys[1] && notes[i].position == 192) { currentNote++; continue; }
                if(keys[2] && notes[i].position == 320) { currentNote++; continue; }
                if(keys[3] && notes[i].position == 448) { currentNote++; continue; }
            }

            if((notes[i].longTime == 0 && notes[i].timing < time) || (notes[i].longTime != 0 && notes[i].longTime < time)) {
                errorTimeout = 2;
                currentNote++;
                continue;
            }

            vec tmp;
            tmp.x = (notes[i].position / 500.0) - 0.3;
            tmp.y = ((notes[i].timing - time) / 500.0) - 1;

            vec tmp2;
            tmp2.x = tmp.x + 0.25;
            tmp2.y = ((notes[i].longTime - time) / 500.0) - 1;
            if(notes[i].longTime == 0) tmp2.y = tmp.y + 0.1;
            draw_rectangle(tmp, tmp2, vec4_create(1, 1, 1, 1));
        }

        for(int i = 0; i < 4; i++)
        {
            if(!keys[i]) continue;
            vec tmp = vec_create(-0.15 + i*0.25, -1);
            draw_rectangle(tmp, vec_add(tmp, vec_create(0.25, 0.2)), vec4_create(1, 1 - errorTimeout, 0 , 1));
        }
        draw_rectangle( vec_create(-0.4, -1), vec_create(-0.3, 1) , vec4_create(1, 1 - errorTimeout, 0 , 1));



    }
}

int last_jump = 0;

void load_map (char * fileName)
{
    char * file = load_file(fileName);
    //char * file = load_file("song/portal.osu");


    bool atBeatmap = false;
    bool nextLine = false;
    char str[7] = {0};
    int strLength = 0;
    int skip = 0;
    int howFarInLine = 0;
    int tempPos = 0;
    int tempTime = 0;
    int tempLong = 0;
    for(int i = 0; i < lengthFile; i++)
    {

        if(amountNotes > 400) break;
        if(atBeatmap)
        {


            if(nextLine)
            {
                if(file[i] != '\n') continue;
                else{   //if at end of line
                    howFarInLine = 0;
                    nextLine = false;

                }
            }
            else if(howFarInLine > 6 && !nextLine)
            {

                notes[amountNotes].position = tempPos;
                notes[amountNotes].timing = tempTime;
                notes[amountNotes].longTime = tempLong;
                printf("Note| position: %i, timing: %i, long: %i\n", notes[amountNotes].position, notes[amountNotes].timing, notes[amountNotes].longTime);

                amountNotes++;
                nextLine = true;
            }
            else if(file[i] == ',' || file[i] == ':') //process field / str
            {
                howFarInLine++;

                if(skip > 0)
                {
                    skip--;
                    continue;
                }

                strLength = 0;
                if(howFarInLine == 1) tempPos = string_int(str);
                if(howFarInLine == 3) tempTime = string_int(str);
                if(howFarInLine == 6) tempLong = string_int(str);
                for(int j = 0; j < 7; j++) str[j] = 0;

                if(howFarInLine == 1) skip = 1;

            }else if(skip > 0)
            {
                continue;
            }else{
                str[strLength] = file[i];
                strLength++;
            }
            continue;
        }

        if(file[i-3] == 'c' && file[i-2] == 't' && file[i-1] == 's' && file[i] == ']')
        {
            atBeatmap = true;
        }
    }
}

char * load_file (char * file)
{
    FILE *fp;
    fp = fopen(file, "r"); // read mode
    rewind(fp);
    fseek(fp, 0, 2);
    int size = ftell(fp);
    rewind(fp);
    char * file_output  = (char *) malloc(sizeof(char) * size);

    if (fp == NULL)
    {
        printf("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }
    int i = 0;
    char ch;
    while(i < size)
    {
        ch = fgetc(fp);
        file_output[i] = ch;
        i++;
    }
    fclose(fp);
    lengthFile = size;
    return file_output;
}

bool start ()
{
    screenx = 800;
    screeny = 600;
    initiate_render();
}

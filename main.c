//#include <stdio.h>
//#include <stdlib.h>

#include "render.c"

bool start();
void main_loop();


char * song = "";
char * path = "";
Sound music;
Sound hitSound;
//#define MAXNOTES 200
int MAXNOTES = 200;

Note * notes;

int amountNotes = 0;
int audioLeadIn = 0;

int main(int argc, char **argv)
{

    printf("Hello world!\n");


    
    //sic = LoadSound("song/blackorwhite/song.mp3");
	
    


	if(argc < 2)
	{
		printf("please specify a song\n");
		return 0;
	}
	//printf("argv %s", argv[1]);
    start();
	
	load_map(argv[1]);
	//background = LoadTexture("song/blackorwhite/123.jpg");
	//LoadImage("song/blackorwhite/123.jpg");

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
bool isHit(int i)
{
	return ((notes[i].longTime == 0 && notes[i].hit) || (notes[i].longTime != 0 && notes[i].hit <= notes[i].longTime * 0.8f));
}

void main_loop ()
{
    float time;
    int currentNote = 0;
    float errorTimeout = 0;
	int playField = 270;
    
    SetSoundVolume(music, 0.2f);
    PlaySound(music);
    float timeOffset = GetTime() * 1000 - audioLeadIn;
    int missedNotes = 0;
    while(frame_update())
    {
        
        //update time
        time = GetTime() * 1000 - timeOffset;
		//nameOf song
		draw_text(vec_create(-0.98f, -0.5f), 30, vec4_create(1,1,1,1),  song);
        //timer
        char num [15];
        //char min [6];
        sprintf(num, "%i:%i", ((int)time/1000) / 60, (int)(time/1000) % 60);
        //sprintf(min, "%i", );
        //strcat(min, num);
        draw_text(vec_create(-0.98f, -0.3f), 50, vec4_create(1,1,1,1), num);
        
        sprintf(num, "missed: %i", missedNotes); 
        draw_text(vec_create(-0.98f, 0), 50, vec4_create(1,1,1,1), num);
        
        sprintf(num, "currentNote: %i", currentNote); 
        draw_text(vec_create(-0.98f, 0.3f), 40, vec4_create(1,1,1,1), num);

        
        //printf("\nDebug: time:%f", time);

        if(errorTimeout > 0) errorTimeout -= delta * 8;
        if(errorTimeout < 0) errorTimeout = 0;
        
        //bars
        for(int i = 0; i < amountKeys; i++)
        {
            if(i %2 == 0)draw_rectangle(vec_create((i / (float)amountKeys), -1), vec_create(((i+1) / (float)amountKeys), 1), vec4_create(1,1,1,0.15f));
        }
        
        

        //printf("jump_moment - millitotal = %i\n", millitotal - jump_moment);
		bool allPreviousNotesHit = true;
        for(int i = currentNote; (notes[i].timing - time < playField * 2 || i - currentNote < 1) && i < amountNotes; i++)
        {
			if(isHit(i) && allPreviousNotesHit) currentNote++;
			else if(!isHit(i)) allPreviousNotesHit = false;
			if(isHit(i)) continue;
			//printf("\nCurrentNote %i", currentNote);
            float margin = 100;
            int position = (notes[i].position -1 )% amountKeys;
            position ++;
            //printf("\n position: %i  note %i", position, currentNote + i);

            if(notes[i].longTime == 0 && notes[i].timing - margin < time) {
                //errorTimeout = 2;
                if(keys[position] - time - timeOffset > -margin) {
					//printf("\n key[%i]-time = %f", position, keys[position]-(time+timeOffset));
					//currentNote++;
					notes[i].hit = true;
					keys[position] == time - 1000;
                    PlaySound(hitSound);
                    //printf("\nPlaying hitsound  CurrentNote %i\n", currentNote);
					continue;
				}
            }
			
			if(notes[i].longTime != 0 && notes[i].timing - margin < time && notes[i].longTime + margin > time)
			{
				
				if(IsKeyDown(keyButtons[position])) {
					printf("\n key[%i] hit %i    %i    %f", position, notes[i].hit, notes[i].hit-(notes[i].longTime-notes[i].timing), GetFrameTime()*10000);
					notes[i].hit -= GetFrameTime() * 1000.0f;
				}
			}
			//printf("\nKey D  %f", keys[position] - (time/1000.0f));

            if((notes[i].longTime == 0 && notes[i].timing < time) || (notes[i].longTime != 0 && notes[i].longTime < time)) {
                errorTimeout = 2;
                notes[i].hit = 1;
                missedNotes++;
				if(notes[i].longTime != 0) notes[i].hit = -100;
                continue;
            }

            vec tmp;
            tmp.x = (position / (float)amountKeys);
            //printf("\nnotes.time %i  actual Time %f", notes[i].timing, time);
            tmp.y = ((notes[i].timing - time) / playField) - 1;
            
            tmp.y *= -1;

            vec tmp2;
            tmp2.x = tmp.x + 0.25;
            
            
            if(notes[i].longTime == 0)tmp2.y = tmp.y - 50.0f/playField;
            else tmp2.y = -1*((notes[i].longTime - time) / playField - 1);
            //printf("DrawRectangle! %f %f, %f %f", tmp.x, tmp.y, tmp2.x, tmp2.y);
            if(notes[i].longTime == 0)draw_rectangle(tmp, tmp2, vec4_create(1, 1, 0.2, 1));
			else draw_rectangle(tmp, tmp2, vec4_create(0.5, 1, 0.2, 1));
        }

        for(int i = 0; i < amountKeys; i++)
        {
            if(IsKeyUp(keyButtons[i])) continue;
            vec tmp = vec_create(i/(float)amountKeys, 0.8);
            draw_rectangle(tmp, vec_add(tmp, vec_create(0.25, 0.2)), vec4_create(1, 1 - errorTimeout, 1 - errorTimeout , 1));
        }
        draw_rectangle( vec_create(-0.4, -1), vec_create(-0.3, 1) , vec4_create(1, 1 - errorTimeout, 1-errorTimeout , 1));



    }
}

int last_jump = 0;
void loadSoundPath(char * str, Sound * sound)
{
    
    char * tmpstr = calloc(sizeof(char), strlen(path) * strlen(str));
    strcat(tmpstr, path);
    strcat(tmpstr, str);
    if( access( tmpstr, F_OK ) != -1 )
        *sound = LoadSound(tmpstr);
    free(tmpstr); 
    return;
    
}

void load_map (char * fileName)
{
    char * file = load_file(fileName);
    notes = malloc(sizeof(Note) * MAXNOTES);
	
	//figuring out path
	if(true)
	{
		int i = 0;
		int amountDashes = 0;
		int positionLastDash = 0;
		while(fileName[i] != '\0')
		{
			if(fileName[i] == '/') 
			{
				amountDashes++;
				positionLastDash = i;
			}
			i++;
		}
        positionLastDash++;
		if(amountDashes != 0)
		{
			path = malloc(sizeof(char)*(positionLastDash+1));	
			for(i = positionLastDash;i >= 0; i--)
				path[i] = fileName[i];
            path[positionLastDash]='\0';
		    printf("\npath: %s\n", path);
		}
	}
    
    loadSoundPath("soft-hitnormal.wav", &hitSound);
    loadSoundPath("normal-hitnormal.wav", &hitSound);
	
	
    //char * file = load_file("song/portal.osu");
    #define MAXPOSITIONS 20
    int positions [MAXPOSITIONS];
    int amountPositions = 0;
    bool atBeatmap = false;
	bool atTitle = false;
    bool nextLine = false;
    char str[7] = {0};
    int strLength = 0;
    int skip = 0;
    int howFarInLine = 0;
    int tempPos = 0;	
    int tempTime = 0;
    int tempLong = 0;
    for(int i = 5; i < lengthFile; i++)
    {

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
                bool taken = false;
                int oldPos = tempPos;
                for(int i =0; i < amountPositions; i++)
                {
                    //printf("amountPositions %i  tempPos %i positions %i\n", amountPositions, tempPos, positions[i]);
                   if(positions[i] == tempPos) 
                   {
                       //printf("\n pos %i tmppos %i  i %i\n", positions[i], tempPos, i);
                       taken=true;
                       tempPos = i;
                       break;
                   }
                }
                if(!taken)
                {
                    positions[amountPositions] = tempPos;
                    amountPositions++;
                    if(amountPositions >= MAXPOSITIONS)
                    {
                        printf("\nError, most likely not an OSU Mania map. No support added for normal maps (yet)\n");   
                        exit(0);
                    }
                    //printf("\n pos %i amountPositions %i\n", tempPos, amountPositions);
                    tempPos = amountPositions;
                }
                notes[amountNotes].position = tempPos;
                notes[amountNotes].timing = tempTime;
                notes[amountNotes].longTime = tempLong;
				if(!tempLong) notes[amountNotes].hit = 0;
                else notes[amountNotes].hit = tempLong-tempPos;
                //printf("Note %i | position: %i, positionNumber %i, timing: %i, long: %i\n", amountNotes, oldPos, notes[amountNotes].position, notes[amountNotes].timing, notes[amountNotes].longTime);

                amountNotes++;
				if(MAXNOTES <= amountNotes+1) 
                {
                    MAXNOTES += 200;
                    notes = realloc(notes, sizeof(Note) * MAXNOTES);
                    
                }
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
        }else if(file[i-3] == 't' && file[i-2] == 'l' && file[i-1] == 'e' && file[i] == ':')
		{
			//getting name
			int length = 0;
			i++;
			int j = i;
			while(file[j] != '\n')
			{
				length++;
				j++;
			}
			//length ++;
			song = malloc(length*sizeof(char));
			j=0;
			while(file[i] != '\n')
			{
				
				song[j] = file[i];
				i++;
				j++;
			}
			song[j-1] = '\0';
			
		}else if (file[i-5] == 'e' && file[i-4] == 'n' && file[i-3] == 'a' && file[i-2] == 'm' && file[i-1] == 'e' && file[i] == ':')
		{
			//getting music file
			int length = 0;
			char * musicFile;
			i+=2;
			int j = i;
			while(file[j] != '\n')
			{
				length++;
				j++;
			}
			length++;
			musicFile = malloc(length*sizeof(char));
			j=0;
			while(file[i] != '\n')
			{
				
				musicFile[j] = file[i];
				i++;
				j++;
			}
			//printf("\nCharacter %i", musicFile[j-1]);
			if(musicFile[j-1] == 13) j--;
			musicFile[j] = '\0';
			char * tmpstr = calloc(length+strlen(path)+1, sizeof(char));
			strcat(tmpstr, path);
			strcat(tmpstr, musicFile);
			printf("\nLoading Music File %s\n", musicFile);
			music = LoadSound(tmpstr);
			free(musicFile);
			free(tmpstr);
		}else if(file[i-3] == 'd' && file[i-2] == 'I' && file[i-1] == 'n' && file[i] == ':')
		{
			//getting audio lead in
			int length = 0;
			i++;
			int j = i;
			while(file[j] != '\n')
			{
				length++;
				j++;
			}
			//length ++;
			char * strtmp = malloc(length*sizeof(char));
			j=0;
			while(file[i] != '\n')
			{
				
				strtmp[j] = file[i];
				i++;
				j++;
			}
			//strtmp[j-1] = '\0';
            for(int k = 0; k < j; k++)
            {
                if(strtmp[k] < '0' || strtmp[k] > '9') continue;
                audioLeadIn = audioLeadIn*10 + (strtmp[k] - '0');
            }
			//audioLeadIn = string_int(strtmp);
            free(strtmp);
			//printf("\n audioLeanIn %i\n", audioLeadIn);
		}else if(file[i-4] == 'M' && file[i-3] == 'o' && file[i-2] == 'd' && file[i-1] == 'e' && file[i] == ':')
		{
			//getting mode of osu file
			i+=2;
			int mode = file[i] - '0';
            printf("\nMode %i, file says %c\n", mode, file[i]);
            if(mode != 3) 
            {
                printf("\nThis map isn't an osu mania map, no support yet for non osu mania maps\n");
                exit(0);
            }
		}
        

        if(file[i-3] == 'c' && file[i-2] == 't' && file[i-1] == 's' && file[i] == ']')
        {
            atBeatmap = true;
        }
    }
    return;
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
    screenx = 1270;
    screeny = 730;
    initiate_render();
}

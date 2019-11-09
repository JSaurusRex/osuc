#include "share.h"
#include "include/glad/glad.h"
#include <GLFW/glfw3.h>
#define GLFW_DLL

#include <sndfile.h>
#include <portaudio.h>



//on windows you will need a 64 bit compiler

GLFWwindow* window;
unsigned int textures [MAX_TEXTURES] = {0};
unsigned int textures_length = 0;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);


bool initiate_render ()
{
    printf("initializing opengl\n");
    if( !glfwInit() )
	{
	    printf("failed to initialize glfw\n");
		return false;
	}

	window = glfwCreateWindow( screenx, screeny, "osuc", NULL, NULL );

	glfwMakeContextCurrent(window);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);



    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return false;
    }
    glViewport(0, 0, screenx, screeny);
    glfwSetKeyCallback(window, key_callback);


    glfwSwapInterval( 0 );

    initiate_sound ();
	return true;
}

#define FRAMES_PER_BUFFER   (512)

typedef struct
{
    SNDFILE     *file;
    SF_INFO      info;
} callback_data_s;


static int callback(    const void *input, void *output,
                        unsigned long frameCount,
                        const PaStreamCallbackTimeInfo* timeInfo,
                        PaStreamCallbackFlags statusFlags,
                        void *userData );

void initiate_sound ()
{
    printf("initializing sound\n");

    SNDFILE *file;
    PaStream *stream;
    PaError error;
    callback_data_s data;

    /* Open the soundfile */
    data.file = sf_open("song/audio.wav", SFM_READ, &data.info);
    if (sf_error(data.file) != SF_ERR_NO_ERROR)
    {
        fprintf(stderr, "%s\n", sf_strerror(file));
        fprintf(stderr, "File: %s\n", "song/audio.mp3");
        return 1;
    }

    /* init portaudio */
    error = Pa_Initialize();
    if(error != paNoError)
    {
        fprintf(stderr, "Problem initializing\n");
        return 1;
    }

    /* Open PaStream with values read from the file */
    error = Pa_OpenDefaultStream(&stream
                                 ,0                     /* no input */
                                 ,data.info.channels         /* stereo out */
                                 ,paFloat32             /* floating point */
                                 ,data.info.samplerate
                                 ,FRAMES_PER_BUFFER
                                 ,callback
                                 ,&data);        /* our sndfile data struct */
    if(error != paNoError)
    {
        fprintf(stderr, "Problem opening Default Stream\n");
        return 1;
    }

    /* Start the stream */
    error = Pa_StartStream(stream);
    if(error != paNoError)
    {
        fprintf(stderr, "Problem opening starting Stream\n");
        return 1;
    }
}

static int callback
    (const void                     *input
    ,void                           *output
    ,unsigned long                   frameCount
    ,const PaStreamCallbackTimeInfo *timeInfo
    ,PaStreamCallbackFlags           statusFlags
    ,void                           *userData
    )
{
    float           *out;
    callback_data_s *p_data = (callback_data_s*)userData;
    sf_count_t       num_read;

    out = (float*)output;
    p_data = (callback_data_s*)userData;

    /* clear output buffer */
    memset(out, 0, sizeof(float) * frameCount * p_data->info.channels);

    /* read directly into output buffer */
    num_read = sf_read_float(p_data->file, out, frameCount * p_data->info.channels);

    /*  If we couldn't read a full frameCount of samples we've reached EOF */
    if (num_read < frameCount)
    {
        return paComplete;
    }

    return paContinue;
}

void load_texture (char * file, int texture, int width, int height)
{
    glGenTextures(1, &textures[texture]);
    glBindTexture(GL_TEXTURE_2D, textures[texture]);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    char * data = load_file(file);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    //glGenerateMipmap(GL_TEXTURE_2D);
    free(data);

    if(textures_length < texture) textures_length = texture;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(action == GLFW_PRESS) update_key(key, true);
    if(action == GLFW_RELEASE) update_key(key, false);
}

void update_key (int key, bool value)
{
    switch(key)
    {
        case GLFW_KEY_D:
            keys[0] = value;
            break;
        case GLFW_KEY_F:
            keys[1] = value;
            break;
        case GLFW_KEY_J:
            keys[2] = value;
            break;
        case GLFW_KEY_K:
            keys[3] = value;
            break;
        case 32:
            keys[4] = value;
            break;
        case GLFW_KEY_LEFT_SHIFT:
            keys[5] = value;
            break;
        case GLFW_KEY_ESCAPE:
            keys[6] = value;
            break;
    }
}


bool frame_update ()
{
    //glFlush();
    int new_time = glfwGetTime() * 1000;
    if(!paused)delta = (float)(new_time-millitotal) / 1000.0f;
    millitotal = new_time;
    if(!paused) millitimer += millitotal - new_time;

    glfwSwapBuffers(window);
    glfwPollEvents();
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    return !glfwWindowShouldClose(window);
}

void draw_texture_texpos (vec pos1, vec pos2, int texture, vec textpos1, vec textpos2);

void draw_texture(vec pos1, vec pos2, int texture)
{
    draw_texture_texpos(pos1, pos2, texture, vec_create(0, 0), vec_create(1, 1));
}

void draw_texture_texpos (vec pos1, vec pos2, int texture, vec textpos1, vec textpos2)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textures[texture]);


    glBegin(GL_QUADS);
        // Bottom left
        glTexCoord2d(textpos1.x, textpos1.y);
        glVertex2d(pos1.x, pos1.y);

        // Top left
        glTexCoord2d(textpos1.x, textpos2.y);
        glVertex2d(pos1.x, pos2.y);

        // Top right
        glTexCoord2d(textpos2.x, textpos2.y);
        glVertex2d(pos2.x, pos2.y);

        // Bottom right
        glTexCoord2d(textpos2.x, textpos1.y);
        glVertex2d(pos2.x, pos1.y);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void draw_rectangle (vec pos1, vec pos2, vec4 color)
{
    glBegin(GL_QUADS);
        // Bottom left
        glColor4d( color.r, color.g, color.b, color.a);
        glVertex2d(pos1.x, pos1.y);

        // Top left
        glVertex2d(pos1.x, pos2.y);


        // Top right
        glVertex2d(pos2.x, pos2.y);

        // Bottom right
        glVertex2d(pos2.x, pos1.y);
    glEnd();
}

void draw_gradient (vec pos1, vec pos2, vec4 color1, vec4 color2, vec4 color3, vec4 color4)
{
    glBegin(GL_QUADS);
        // Bottom left
        glColor4d( color1.r, color1.g, color1.b, color1.a);
        glVertex2d(pos1.x, pos1.y);

        // Top left
        glColor4d( color2.r, color2.g, color2.b, color2.a);
        glVertex2d(pos1.x, pos2.y);


        // Top right
        glColor4d( color3.r, color3.g, color3.b, color3.a);
        glVertex2d(pos2.x, pos2.y);

        // Bottom right
        glColor4d(color4.r, color4.g, color4.b, color4.a);
        glVertex2d(pos2.x, pos1.y);
    glEnd();
}

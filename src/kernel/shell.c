#include "pch.h"
#include "shell.h"
#include "screen.h"

static size_t index;
static char command_buf[COMMAND_BUFFER_SIZE];

void reset_buffer()
{
    index = 0;
    memset_tool(command_buf, 0, COMMAND_BUFFER_SIZE);
}

void handle_input(char *input)
{
    // This function will be called when Enter is pressed by the user
    screen_print(input, 0);
}


void handle_shell(char content, uint8_t attr)
{
    char string[] = {content, '\0'};
    screen_print(string, attr);

    command_buf[index++] = content;

    if (content == '\n')
    {
        command_buf[index] = '\0';
        handle_input(command_buf);
        reset_buffer();
    }
}
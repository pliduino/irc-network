
#include <string.h>
#include <stdint.h>

uint8_t channel_name_is_valid(char *channel_name)
{
  if (channel_name[0] != '&' && channel_name[0] != '#')
    return 0;

  int i = 0;
  while (channel_name[i] != '\0')
  {
    if (i == 200)
      return 0;
    char ch = channel_name[i];
    if (ch == ' ' || ch == 7 || ch == ',')
      return 0;
    i++;
  }

  return 1;
}
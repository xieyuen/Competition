#include <RobotLab_Plus.h>

int length = 15; // the number of notes
char notes[] = "ccggaagffeeddc "; // a space represents a rest
int beats[] = { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 4 };
int tempo = 300;
int pin=13;
void playNote(char note, int duration) 
{
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
  int tones[] = {262, 294, 330, 349, 392, 440, 494, 523 };
  //int tones[] = {523, 587, 659, 698, 784, 880, 988,1046 };
  // play the tone corresponding to the note name
  for (int i = 0; i < 8; i++) 
  {
    if (names[i] == note) 
    {
      SetBuzzer(pin, tones[i], duration);
    }
  }
}
void setup() 
{
}

void loop() 
{
  for (int i = 0; i < length; i++) 
  {
    if (notes[i] == ' ')
    {
      delay(beats[i] * tempo); // rest
    } else
    {
      playNote(notes[i], beats[i] * tempo);
    }

    // pause between notes
    delay(tempo / 2); 
  }
}

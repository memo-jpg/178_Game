#include "_sounds.h"

_sounds::_sounds()
{
    //ctor
}

_sounds::~_sounds()
{
    //dtor
    sndEng->drop();
}
void _sounds::playMusic(char* fileName)
{
    //sndEng->play2D(fileName, true);

    wav = sndEng->addSoundSourceFromFile(fileName);
    wav->setDefaultVolume(0.25f);
    //engine->setSoundVolume(0.02); //worked, but changed volume for all sounds... which makes sense from the name...
    sndEng->play2D(fileName, true); //file will repeat
}

void _sounds::playSounds(char* fileName)
{
    sndEng->play2D(fileName, false, false);
}

void _sounds::pauseSound(char* fileName)
{
    sndEng->play2D(fileName, true, false);
}

void _sounds::initSound()
{
    if(!sndEng) std::cout<<"ERROR** Sound Engine did not Start"<< std::endl;
/*
    if(!sndEng)
    {
        cout << "ERROR: ***The sound Engine could not start*** \n";
        return 0; //if no good, quits game if no music
                    //fix in case if you want to play w/o music
    }
    return 1; //if all good
*/
}

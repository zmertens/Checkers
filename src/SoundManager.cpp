#include "SoundManager.hpp"

using std::string;
using std::cout;
using std::endl;

SoundManager::SoundManager()
{
}

SoundManager::~SoundManager()
{
    cout << "Releasing audio memory... ";
    for(unsigned int i = 0; i < soundBufferArray.size(); ++i)
    {
        soundArray[i]->stop();
        delete(soundArray[i]);
        delete(soundBufferArray[i]);
    }
    soundArray.clear();
    soundBufferArray.clear();
    cout << "OK" << endl;
}

 SoundManager* SoundManager::getSoundManager()
 {
   static SoundManager singleton;
   return &singleton;
 }

void SoundManager::addSound(string fileName)
{
    cout << "Loading sound: " << fileName << "...\n";
    sf::SoundBuffer* newSoundBuffer = new sf::SoundBuffer;
    newSoundBuffer->loadFromFile(fileName);
    soundBufferArray.push_back(newSoundBuffer);

    sf::Sound* newSound = new sf::Sound;
    newSound->setBuffer(*newSoundBuffer);
    soundArray.push_back(newSound);
}

void SoundManager::playSound(int n)
{
    if (soundArray[n]->getStatus() != sf::Sound::Playing)
        soundArray[n]->play();
}

void SoundManager::stopSound(int n)
{
    if (soundArray[n]->getStatus() == sf::Sound::Playing)
        soundArray[n]->stop();
}

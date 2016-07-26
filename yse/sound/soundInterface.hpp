/*
  ==============================================================================

    sound.h
    Created: 28 Jan 2014 11:50:15am
    Author:  yvan

  ==============================================================================
*/

#ifndef SOUNDINTERFACE_H_INCLUDED
#define SOUNDINTERFACE_H_INCLUDED

#include "../classes.hpp"
#include "../headers/defines.hpp"

#if defined PUBLIC_JUCE
#include "JuceHeader.h"
#endif

namespace YSE {
  

  /**
      A sound object is needed for every kind of sound you want to use. Sounds can use
      audio files (wav, ogg, flac, and more on some systems) or can be linked to a DSP
      source. Sounds can be mono, stereo or multichannel.

      If a soundfile is streamed, it will use its own soundbuffer. Otherwise YSE will
      create a new buffer only if needed or re-use and existing buffer if another file
      with the same filename has already been loaded. Unloading of soundfiles and clearing
      memory is done internally: when a buffer is not used by any sound any more, it will be
      flagged for deletion.
      */

  class API sound {
  public:
    sound();
    ~sound();

    // Sound interfaces cannot be copied. The implementation needs access to the 
    // interface object. To do this, the address of the interface must not change.
    sound(const sound&) = delete;

    /** Create a filebased sound and register it with the soundsystem. Other functions will not work
        as long as a sound hasn't been created. In debug mode, an assertion will be called if
        you try to do so. On the other hand, create cannot be called twice. If you need
        another sound, create a new sound object.

        @param fileName   This can be an absolute path or something relative to the
        working directory.
        @param channel    The sound channel you want to attach the sound to. Sounds can
        be moved to other channels at any time. If no channel is provided,
        the sound will be added to the global channel (mainMix).
        @param loop       Use true to loop this sound continuously. If not provided, the sound
        will not loop.
        @param volume     The initial volume to play this sound. This should be a value between
        0.0 and 1.0. Default is 1.0.
        @param streaming  Whether or not this sound should be streamed. Defaults to false.
        Streaming is mainly intended for big audio files that are only used
        by a single sound. When a sound is used repeatedly, avoid streaming.
        */
    sound& create(const char * fileName, channel * ch = nullptr, Bool loop = false, Flt volume = 1.0f, Bool streaming = false);

    /** Create a sound based on an audiobuffer. This function is useful to play soundfiles which are loaded to
        be manipulated as well as played. 

        @param audioBuffer A reference to the buffer. Buffers can be passed to multiple sounds.
        @param channel    The sound channel you want to attach the sound to. Sounds can
        be moved to other channels at any time. If no channel is provided,
        the sound will be added to the global channel (mainMix).
        @param loop       Use true to loop this sound continuously. If not provided, the sound
        will not loop.
        @param volume     The initial volume to play this sound. This should be a value between
        0.0 and 1.0. Default is 1.0.
        */

    sound& create(YSE::DSP::buffer & buffer, channel * ch = nullptr, Bool loop = false, Flt volume = 1.0f);

    /** Small variation of the previous function, but specialized for multichannel sources.
    */

    sound& create(MULTICHANNELBUFFER & buffer, channel * ch = nullptr, Bool loop = false, Flt volume = 1.0f);

    /** Create a DSP sound and register it with the soundsystem. In contrast to a filebased sound,
        the source if this soundobject will be generated by a dspSourceObject you provide yourself.

        @param dsp      You provide a reference to an object for sound generation. Such objects must
        be based on dspSourceOBject.
        @param channel  The channel to attach the sound to. If no channel is provided, the sound will
        be added to the global channel (mainMix).
        @param volume   The volume at which to start playing this sound.
        */
    sound& create(DSP::dspSourceObject &  dsp, channel * ch = nullptr, Flt volume = 1.0f);

    /** Registers a synth with the soundsystem. In contrast to a filebased sound,
    the source if this soundobject will be generated by a synth you provide yourself.

    @param synth      You provide a reference to an synth for sound generation.
    @param channel  The channel to attach the sound to. If no channel is provided, the sound will
    be added to the global channel (mainMix).
    @param volume   The volume at which to start playing this sound.
    */
    sound& create(SYNTH::interfaceObject &  synth, channel * ch = nullptr, Flt volume = 1.0f);


#if defined PUBLIC_JUCE
    /**
        This is an extra create function which enables you to pass a Juce InputStream with audio data
        to YSE. This is usefull if you use Juce to create an application. But since it creates an additional
        depency on the Juce library, it has to be enabled by including the preprocessor definition
        PUBLIC_JUCE in your project settings.
    */
    sound& create(juce::InputStream * source, channel * ch = nullptr, Bool loop = false, Flt volume = 1.0f, Bool streaming = false);
#endif

    /*
      Checks if there is an implementation linked to this interface. You can use this for debugging, but safely assume
      this returns true for the lifetime of the interface. It's not needed to check against this function before
      using the interface.
    */
    Bool isValid();

    /**
      Set the position of this sound in the virtual space.
      */
    sound& pos(const Pos &v);

    /**
      Get the current position of this sound.
      */
    Pos pos();

    /**
        This is only useful for multichannel sounds. If a sound has more than one channel,
        the channels can be spread out over the stereo or surround field.
        This defines how much space there is between the position of the channels.
        */
    sound& spread(Flt value);

    /**
        Returns how much a multichannel sound is spread out in space.
        */
    Flt spread();

    /**
        The speed at which a sound plays back alters it's pitch. The actual speed will also
        be affected by velocity (doppler effect) unless doppler is disabled.

        Internally this value will never be set entirely to zero, because that would result
        in static output that might damage speakers.

        A negative value is also possible and will result in the sound being played backwards.
        Streaming sounds will not be played backwards though, because this would require an
        insane amount of disk activity. (This might change later on because SSD disks do not
        suffer from this.
        */
    sound& speed(Flt  value);

    /** Returns the current speed at which the sound is playing.
    */
    Flt speed();

    /**
      Set the 'size' of the sound. This defines from how far away in the virtual space a sound
      will be heard.
      */
    sound& size(Flt  value);

    /** Returns the current size of the sound, ie. how far away it will be heard.
    */
    Flt size();

    /** Set if this sound is looping or one-shot. Should be obvious, right?
    */
    sound& looping(Bool value);

    /** If this sound is looping or one-shot.
    */
    Bool looping();

    /**
      Sets the volume for this sound. You can use this as a fader by supplying a time value.

      @param value The target volume.
      @param time  If supplied, do a smooth change to the target volume over 'time' milliseconds. (Default is 0.)
      */
    sound& volume(Flt value, UInt time = 0);

    /**
      Get the current volume. This can be different from the current target volume if a time has been supplied
      when setVolume() is used.
      */
    Flt volume();

    /**
      Fadeout the over 'time'  milliseconds. Then stop the sound.
      */
    sound& fadeAndStop(UInt time);

    /**
      Play this sound.
      */
    sound& play();

    /**
      Return true if the sound is currently playing.
      */
    Bool isPlaying();

    /**
      Pause this sound. Starting it again will continue from the current position.
      */
    sound& pause();

    /**
      Return true if the sound is currently paused.
      */
    Bool isPaused();

    /**
      Stop this sound. The position in the source file will be reset to zero. If started again, the sound
      will start from the beginning.
      */
    sound& stop();

    /**
      Return true if the sound is currently stopped.
      */
    Bool isStopped();

    /**
      Pauses the sound if playing, continues if paused, starts if stopped.
      */
    sound& toggle();

    /**
      Restart a sound from the beginning, even if its current position was someplace else.
      */
    sound& restart();

    /**
      Set the current plahing position in the file. Don't get confused here with setPosition,
      which sets the position in the virtual space where the sound will be rendered.

      @param value The new position in samples. Must be between 0 and the length of the file.
      */
    sound& time(Flt value);

    /**
      Get the current playing position in the file. Don't get confused here with getPosition,
      which gets the position in the virtual space where the sound will be rendered.

      @return the current position in samples.
      */
    Flt  time();

    /**
      Get the length of the file.

      @return the length in samples.
      */
    UInt length(); // sound length in samples

    /**
      Make a sound relative to the Listener() object. This is YSE's alternative to '2D' sounds.
      Relative sounds can still move, but they move along with the player. By default this is
      how Gui sounds and music are used in most gaming sound setups.

      But there's more: by making the sounds relative instead of fixed, you can still to other
      stuff with them. The sound can still be moving.
      */
    sound& relative(Bool value);

    /**
      See if a sound is relative to the Listener() object.
      */
    Bool relative();

    /**
      Enable / disable doppler for this sound.
      @see System().setDopplerScale() for global doppler behaviour
      */
    sound& doppler(Bool value);

    /**
      See if doppler is enabled for this sound
      */
    Bool doppler();

    /**
    This is a convenience function equal to setRelative(true).setPosition(0).setDoppler(false);
    */
    sound& pan2D(Bool value);


    Bool pan2D();

    /**
      Will be true if the sound is streamed (from disk or over the network) instead of played
      from memory.
      */
    Bool isStreaming();

    /**
      Will be true if the sound is ready to be played (or if it is already playing). This can take a while
      if a sound is not streaming because loading sounds into memory is done in a seperate time as not to
      obstruct the audio callback.
      Not that it is not needed to wait for this result to be true before you can use most other functions
      with this sound. For instance, if you use the play() function, the sound will be played when it
      becomes ready.
      */
    Bool isReady();

    /**
      This will enable sound occlusion for this sound. Remember to setup an occlusion callback function
      in System() first.
      */
    sound& occlusion(Bool value);

    /**
      Returns true if sound occlusion is active for this sound.
      */
    Bool occlusion();

    /**
      Moves the sound to another channel

      @param target The target channel
      */
    sound& moveTo(channel & target);

    sound& setDSP(DSP::dspObject * value); DSP::dspObject * getDSP(); // attach a dsp object to this sound

      

  private:
    SOUND::implementationObject * pimpl;

    // These values keep the last set value and are used by getters
    // so that we don't have to query the implementation
    Pos  _pos;
    Flt  _spread;
    Flt  _volume;
    Flt  _speed;
    Flt  _size;
    Bool _loop;
    Bool _relative;
    Bool _doppler;
    Bool _pan2D;
    Bool _occlusion;

    UInt _fadeAndStopTime;
    DSP::dspObject * _dsp;
    channel * _parent;

    // these are frequently updated by the implementation
    aBool _streaming;
    aUInt _length;
    aFlt  _time;
    std::atomic<SOUND_STATUS> _status; // what it is currently doing

    friend class SOUND::implementationObject;
  };
}





#endif  // SOUND_H_INCLUDED

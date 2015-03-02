# Interface of Nature (ION)

ION is a program package for creation of electronic music, mainly generative
electronic and electro-acoustic music. It is a bit obsolete and completely
undocumented. But it is easy to work with. (No, it is not.) Its list of
features is strange for a program of this sort. Yet you may like it if
you're freaky enough. I should warn you that I will not work on it anymore
as I've lost any interest for imperative languages other than assembly and C
(it is in C++!).

## Programs

ION consists of the following programs:

- **Scores Editor** creates scores and translates them into `.wav` files.

- **Sonorities Editor** creates primitive instruments.

- **Generator** generates scores that you can then translate.

- **Parameter Lists Editor** creates parameter lists. It's kinda file
  format.

- **Wave Master**. Don't use it, use SoundForge or whatever.

- **ION Launcher**. Yes the icons are funny, I made them myself.

## Known issues

ION had lots of bugs in the past (in is in C++!), but now it is kinda good
boy, but anyway here is a list of bad things about ION:

- When you have really big project ION needs quite a minute to load
  it.

- Audio quality of preview is worse than what you'll get after translation,
  so fear not.

- ION works like a sampler, it creates timbres from samples and then plays
  them with different speed, and guess what, it is stupid enough to use
  linear approximation. For non-technical reader: it introduces some
  harmonics that are audible in certain cases, especially if you work with
  signals that have not so many harmonics initially (sine). Personally I
  don't cry because of this, I like the shit, but you can use dither, really
  bold dither I would say. But it gets a little noisy. Personally I like
  background noise, but you may not, so if you don't like noise ION is
  probably not for you.

- ION is not interactive. It is for producers. You do a track and put it
  out.

- ION does not work with MIDI at all. Why? MIDI is too piano oriented and I
  don't like standard temperament :-) In fact, the way how ION organizes
  music is so different from the conventional, so there is no possible way
  you can work with MIDI in ION.

- ION does not provide effects. You apply them in your DAW. ION knows its
  place in work-flow.

- ION does not use SSE4 because it (ION) is based on wrong principles of
  functioning. It can do its work only sample by sample, not by packs. But
  it is still pretty fast.

- There is probability that you'll meet a horned toad while using the app.

## License

Copyright (c) 2014 Mark Karpov

Distributed under GNU GPL, version 3.

**Warning!** The code itself is free as in freedom (and in fact as in free
beer), but it needs linking with proprietary libraries to work. And it must
be compiled using a proprietary compiler. So this is what it is, a piece of
shit that I put here anyway.

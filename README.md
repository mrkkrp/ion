# Interface of Nature (ION)

ION is a program package for creation of electronic music, mainly generative
electronic and electro-acoustic music. It is a bit obsolete and completely
undocumented. But it is easy to work with. (No, it is not.) Its list of
features is strange for a program of this sort. Yet you may like it if
you're freaky enough. I should warn you that I will not work on it anymore
as I've lost any interest for imperative languages other than assembly and C
(it is in C++!).

This stuff was written partly because I wanted to write all parts of such a
system myself, so it contains implementation of things that can be done
better with help of good libraries, it's pretty usable anyway.

## Programs

ION consists of the following programs:

* **Scores Editor** creates scores and translates them into `.wav` files.

* **Sonorities Editor** creates primitive instruments.

* **Generator** generates scores that you can then translate.

* **Parameter Lists Editor** creates parameter lists. It's kinda file
  format.

* **Wave Master**. Don't use it, use SoundForge or whatever.

* **ION Launcher**. Yes the icons are funny, I made them myself.

## Installation

ION is written with commercial IDE C++ Builder. It makes compilation form
source more difficult (or to be precise, it makes you get the commercial
compiler). Version XE 2 or later is required.

1. You should download or clone ION repository. To clone the repository
execute the following command:

    ```
    $ git clone https://github.com/mrkkrp/ion.git
    ```

2. If your version of C++ Builder is different form XE 5, or if you've
installed the IDE in non-default directory, you have to edit `make.cmd` in
order to correct some pathes on top of the file.

3. Now you're ready to run `make.cmd` script. This script will compile
entire ION system and form ready-to-use application in `bar`
subdirectory. However, for normal functioning one should generate installer
and then install ION with the installer.

4. To generate installer we use Inno Setup. Thus, you should install Inno
Setup (it is free software). Use `ion.iss` file to produce ION installer.

5. Run the installer.

## Known issues

ION had lots of bugs in the past (it is in C++!), but now it's kinda good
boy, but anyway here is a list of bad things about ION:

* When you have really big project ION needs quite a minute to load
  it.

* Audio quality of preview is worse than what you'll get after translation,
  so fear not.

* ION works like a sampler, it takes samples and then plays them with
  different speed, and guess what, it is stupid enough to use linear
  approximation. For non-technical reader: it introduces some harmonics that
  are audible in certain cases, especially if you work with signals that
  have not so many harmonics initially (sine). Personally I don't cry
  because of this, I like the shit, but you can use dither, really bold
  dither I would say. But it gets a little noisy. Personally I like
  background noise, but you may not, so if you don't like noise ION is
  probably not for you.

* ION is not interactive. It is for producers. You do a track and put it
  out.

* ION does not work with MIDI at all. Why? MIDI is too piano oriented and I
  don't like standard temperament :-) In fact, the way how ION organizes
  music is so different from the conventional, so there is no possible way
  you can work with MIDI in ION.

* ION does not provide effects. You apply them in your DAW. ION knows its
  place in work-flow.

* ION does not use SIMD instructions (like SSE) because it (ION) is based on
  wrong principles of functioning. It can do its work only sample by sample,
  not by packs. But it is still pretty fast.

* ION cannot open files that have non-Latin characters in their names;

* Chances are you'll meet a horned toad while using the app.

To be honest, ION is pretty obsolete. Now I use my another programs MIDA and
ALGA to do the same sort of thing (but in fabulous quality, since MIDA and
ALGA just create data that controls high-end quality plugins and DAWs, or
whatever stuff you have). But ION still knows some tricks.

## How to Improve ION

If you want to improve ION and continue its development, I advise the
following improvements:

* use boost instead of STL;

* try to rewrite some parts in more functional style;

* use some well-known library to work with WAVE, abandon my library;

* use XML instead of parameter lists, abandon my library;

* it would be great if it could be compiled with GCC (Qt for GUI), so people
  could build it and use without buying any stuff.

## License

Copyright (c) 2014 Mark Karpov

Distributed under GNU GPL, version 3.

**Warning!** The code itself is free as in freedom (and in fact, as in free
beer), but it needs linking with proprietary libraries to work. And it must
be compiled using a proprietary compiler. So this is what it is, a piece of
shit that I put here anyway.

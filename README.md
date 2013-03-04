speedy
======

Speedy is a basic bitboard chess move generator framework in C. It was written to try and make very fast tree search, by inlining the search method into the move generator. This was accomplished using some rather heinous macros. I have since seen the light and have accepted that better languages are needed before things like this become reasonable. I was also experimenting with short, abbreviated variable/file names. Yes it's terrible, sorry about that.

It doesn't seem to compile right now, and it's incomplete. But I did end up using this within Rondo (my world-championship-winning chess program) for a use that I won't go into, since it was pretty much Anthony Cozzie's idea.

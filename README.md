# Textolitaire 2
Text-based Solitaire for Linux terminal (curses version).

![Screenshot](pic/textolitaire-v2.png)

To compile and run the game, use:

```
make run
```

## Important notes
- Make sure you have the wide-char version of `ncurses` library installed
- Use terminal with UTF-8 support
- Some font sets may not handle the used symbols correctly.
  Most notably, the card color symbols (hearts, spades, ...)
  may be wider than a grid cell and overlap with other text.
  Experiment with various font settings to find the ideal balance.


# Textolitaire 1

Original Textolitaire written for plain Linux terminal

![Screenshot](pic/textolitaire-v1.png)

To compile and run the game, use:

```
make run1
```

## Important notes
- The default color scheme was designed for a light background terminal.
  For darker terminals, you may find a different color scheme better.
  There are several color schemes to choose from (shortcut `s`).
  If none of these work, try editing the `config.h` file.
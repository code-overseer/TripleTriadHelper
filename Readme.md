# TripleTriad Helper

Want to farm cards in Final Fantasy VIII? Tired of trying so hard playing TripleTriad and constantly getting beat by a stupid machine? Use my TripleTriad Helper terminal app to help you win the game.

## Solver Info
The hint provides the potential score for the next 3 turns, which means this also covers the possibility of setting traps to take advantage of the Same, Same Wall or Plus rule. Although the current version allows you to play both Open and Closed games, the solver currently only works for Open games. There should be guides available to spread the Open rule to a region so that you can use the solver to beat the CPU.

The solver uses the alpha-beta pruning algorithm to look 3 turns ahead. The reason for the low depth is due to the branching factor of the decision tree. Suppose we start a game where the Player makes the first move. Given 5 cards to choose from and 9 blank positions on the board, there are a total of 45 child nodes. In the next move, the CPU will have 5 cards and 8 blank positions, so 40 child nodes for each of the 45 previous child nodes; thats 1800 nodes just to look 1 turn ahead. This therefore makes it a bit intensive for the first and possibly second move simply due to the sheer number of possibilities to explore. Multithreading is used to parallelize each of the 1st level branch to help speed it up; but even so, on a quad-core Macbook Pro it still takes a few seconds to explore the next 3 turns. As you can imagine, for a Closed game, the CPU could have access to over 80+ card possibilities (excluding rare cards) which would make it even more intensive to explore the tree.

Even if only three turns are explored, it would still be enough as it is unlikely for a CPU to look so far ahead; it would make the game unwinnable if that was the case.

## Getting Started


### Prerequisites

Ncurses is used to draw the GUI on terminal. Other packages are already included in the include directory. Credits to jarro2783 for his cxxopts and ben-strasser for his csv parser, which I used to parse the card data.

### Installing

Just build with CMake, in the TripleTriadHelper directory
```
mkdir build && cd build && cmake ..
```

Then make the executable
```
make
```

### Command-Line Arguments
| Option           | Arguments          | Description                 | Optional |
| ---------------- | ------------------ | --------------------------- | -------- |
| `-s --same`      |                    | Activate Same Rule          | Yes      |
| `-p --plus`      |                    | Activate Plus Rule          | Yes      |
| `-w --wall`      |                    | Activate Wall Rule          | Yes      |
| `-p --elemental` | "NEFNNNPNN"        | Activate Elemental Rule     | Yes      |
| `-b --blue`      | "card1,card2..."   | Supply Player cards         | No       |
| `-r --red`       | "card1,card2..."   | Supply CPU cards            | Yes      |
| `-t --turn`      | `R` or `B`         | First turn                  | No       |

If the CPU cards are not supplied, a Closed game will be started. If elemental rule is enable, the elements on the board need to be inputed as indicated on the table. The order goes from left to right followed by up to down in the 9x9 grid.

### Element Characters

| Character | Element |
| --------- | ------- |
| A         | Water   | 
| F         | Fire    | 
| T         | Thunder | 
| E         | Earth   | 
| P         | Poison  | 
| H         | Holy    | 
| I         | Ice     | 
| N         | None    | 
| W         | Wind    |

### Example

```
./TripleTriad -sw -e "NNNFNEANN" -r \
"Iron Giant,Biggs Wedge,Blitz,Turtapod,Forbidden" \
-b "Edea,Quistis,Bahamut,Diablos,Leviathan" -t R
```

The following output should be displayed:
```
RED*                        BLUE
+---+                       +---+
| 6 |                       | A |
|5 5|                       |3 A|
| 6 |                       | 3 |
+---+                       +---+
+---+    +---++---++---+    +---+
| 8 |    |   ||   ||   |    | 9 |
|8 4|    | 0 || 1 || 2 |    |2 6|
| 4 |    |   ||   ||   |    | A |
+---+    +---++---++---+    +---+
+---+    +---++---++---+    +---+
| 1T|    |F  ||   ||E  |    | A |
|7 5|    | 3 || 4 || 5 |    |6 8|
| 4 |    |   ||   ||   |    | 2 |
+---+    +---++---++---+    +---+
+---+    +---++---++---+    +---+
| 2 |    |A  ||   ||   |    | 5 |
|7 3|    | 6 || 7 || 8 |    |3 A|
| 6 |    |   ||   ||   |    | 8 |
+---+    +---++---++---+    +---+
+---+                       +---+
| 6 |                       | 7A|
|2 6|                       |7 A|
| 3 |          5:5          | 1 |
+---+                       +---+

0. Iron Giant               0.Edea
1. Biggs Wedge              1.Quistis
2. Blitz                    2.Bahamut
3. Turtapod                 3.Diablos
4. Forbidden                4.Leviathan

Input: [Card no]:[Pos no]
For Blue turn only, input: hint
For non-Open game, opponent input: [CardName]:[Pos no]


```

The asterisk in `RED*` indicates that it is the CPU's turn. As the short input indicates, just input the card index followed by colon and position index to place a card on the board. For a Closed game, input should be card name followed by colon and position index when it is Red's turn. 

On the player's (Blue) turn, inputing `hint` wil generate a 9-row table displaying the possible moves sorted by the potential score at the end of the third turn.

```
RED                         BLUE*
+---+                       +---+
| 8 |                       | A |
|8 4|                       |3 A|
| 4 |                       | 3 |
+---+                       +---+
+---+    +---++---++---+    +---+
| 1T|    | 6 ||   ||   |    | 9 |
|7 5|    |5R5|| 1 || 2 |    |2 6|
| 4 |    | 6 ||   ||   |    | A |
+---+    +---++---++---+    +---+
+---+    +---++---++---+    +---+
| 2 |    |F  ||   ||E  |    | A |
|7 3|    | 3 || 4 || 5 |    |6 8|
| 6 |    |   ||   ||   |    | 2 |
+---+    +---++---++---+    +---+
+---+    +---++---++---+    +---+
| 6 |    |A  ||   ||   |    | 5 |
|2 6|    | 6 || 7 || 8 |    |3 A|
| 3 |    |   ||   ||   |    | 8 |
+---+    +---++---++---+    +---+
                            +---+
                            | 7A|
                            |7 A|
               5:5          | 1 |
                            +---+

No.   Card           Position Potential
1     Leviathan      8        6
2     Bahamut        8        6
3     Bahamut        1        6
4     Leviathan      1        6
5     Leviathan      7        6
6     Bahamut        7        6
7     Leviathan      6        6
8     Diablos        4        6

```

As the table indicates, the potentials are quite similar for all the moves shown, it is up to you to choose which move. Press any key to close the table and return to the card input. Note that the table data is not stored so if you close it by mistake it needs to recompute.

## Authors

* **Bryan Wong** [muramasa2402](https://github.com/muramasa2402)

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* Thank you Jesusausage for our discussion on this.

## Future

I'm figuring out how to make it solve faster ( Not copying the Board for each node would probably be a good start) and also how to run the solver for a Closed game.
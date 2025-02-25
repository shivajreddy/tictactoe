

enum Tile {
  TILE_X, // 0
  TILE_O, // 1
};

enum GameStyle {
  TRADITIONAL, // 3x3
  GOMUKU,      // 15x15
};

typedef struct Board {
  int size;
} Board;

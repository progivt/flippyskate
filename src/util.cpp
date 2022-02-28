int clamp(int x, int min, int max) {
    x = x > max ? max : x;
    return x < min ? min : x;
}

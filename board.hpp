class Board
{
private:
    class Piece
    {
    private:
        sf::Texture tex;//the texture used by the sprite
        sf::Sprite sprite;//the sprite used to draw the piece

    public:
        //functions used for the function pointer member variable
        static std::vector<sf::Vector2i> ShowKnightMoves(int x, int y, int sign);
        static std::vector<sf::Vector2i> ShowPawnMoves(int x, int y, int sign);
        static std::vector<sf::Vector2i> ShowKingMoves(int x, int y, int sign);
        static std::vector<sf::Vector2i> ShowQueenMoves(int x, int y, int sign);
        static std::vector<sf::Vector2i> ShowBishopMoves(int x, int y, int sign);
        static std::vector<sf::Vector2i> ShowRookMoves(int x, int y, int sign);

    public:
        Piece() = default;
        Piece& operator=(const Piece&) = default;
        Piece(std::string filename, std::vector<sf::Vector2i> (*ShowMoves)(int x, int y, int sign));//constructor used for initializing a piece
        void Draw(int x, int y, sf::RenderWindow &wind);
        std::vector<sf::Vector2i> (*ShowMoves)(int x, int y, int sign);//the idea here is that I want a function pointer that stores the function used to show the moves of the piece
    };

    static char chesstable[8][9];//the actual representation of the pieces from the chess table
    std::unordered_map<char, Piece*> piece;//a mapping from the characters to the actual pieces
    std::vector<sf::Vector2i> selectedPieceMoves;//the moves that the selected piece can make, used for drawing
    sf::Vector2i selectedPiece;//the table coords of the selected piece
    bool turn;//bool used to keep track of whose turn it is to move
    static bool ks,rls,rrs;//bools used for castling purposes king start rook left start rook right start

    void filterMoves(sf::Vector2i p, std::vector<sf::Vector2i>& moves, int sign);//used to remove the illegal moves from the possible moves

public:
    Board();//constructor used for initializing member variables
    void Draw(sf::RenderWindow &wind);//function used to draw the board
    void Click(sf::Vector2i mpos);//function used to interpret a click
    void SockFish();//function used by the board to play a move
};
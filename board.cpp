#include "libs.hpp"

char Board::chesstable[8][9] = {
    "RNBKQBNR",
    "PPPPPPPP",
    "        ",
    "        ",
    "        ",
    "        ",
    "pppppppp",
    "rnbkqbnr"};

bool Board::ks = true;
bool Board::rls = true;
bool Board::rrs = true;

Board::Board() : turn(0)
{
    this->piece['R'] = new Piece("./resources/wrook.png", Piece::ShowRookMoves);
    this->piece['N'] = new Piece("./resources/wknight.png", Piece::ShowKnightMoves);
    this->piece['B'] = new Piece("./resources/wbishop.png", Piece::ShowBishopMoves);
    this->piece['K'] = new Piece("./resources/wking.png", Piece::ShowKingMoves);
    this->piece['Q'] = new Piece("./resources/wqueen.png", Piece::ShowQueenMoves);
    this->piece['P'] = new Piece("./resources/wpawn.png", Piece::ShowPawnMoves);
    this->piece['r'] = new Piece("./resources/rook.png", Piece::ShowRookMoves);
    this->piece['n'] = new Piece("./resources/knight.png", Piece::ShowKnightMoves);
    this->piece['b'] = new Piece("./resources/bishop.png", Piece::ShowBishopMoves);
    this->piece['k'] = new Piece("./resources/king.png", Piece::ShowKingMoves);
    this->piece['q'] = new Piece("./resources/queen.png", Piece::ShowQueenMoves);
    this->piece['p'] = new Piece("./resources/pawn.png", Piece::ShowPawnMoves);
}

void Board::Draw(sf::RenderWindow &wind)
{
    // drawing the tiles of the board
    sf::RectangleShape tile(sf::Vector2f(100, 100));
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            tile.setFillColor((i + j) % 2 == 1 ? sf::Color(133, 89, 1) : sf::Color(240, 188, 98));
            tile.setPosition(j * 100, i * 100);
            wind.draw(tile);
        }
    }

    // drawing the pieces of the board
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {

            if (this->chesstable[i][j] != ' ')
            {
                this->piece[chesstable[i][j]]->Draw(j * 100, i * 100, wind);
            }
        }
    }

    // drawing the possible moves of the selected piece
    sf::CircleShape circle(25.0f);
    circle.setFillColor(sf::Color(120, 120, 120, 122));
    for (int i = 0; i < this->selectedPieceMoves.size(); i++)
    {
        circle.setPosition(this->selectedPieceMoves[i].x * 100 + 25, this->selectedPieceMoves[i].y * 100 + 25);
        wind.draw(circle);
    }
}

void Board::Click(sf::Vector2i mpos)
{
    //we first check if the coordonates are inside the window
    if (this->turn && mpos.x >= 0 && mpos.x < 800 && mpos.y >= 0 && mpos.y < 800)
    {
        //we calculate the tile we pressed on and we check if we clicked on a piece or on a square where the selected piece can move
        sf::Vector2i tabcoor(mpos.x / 100, mpos.y / 100);
        if (this->chesstable[tabcoor.y][tabcoor.x] >= 'a')
        {
            //if we clicked on a piece we change the selected piece to the one we clicked on and then we update the selected piece moves
            this->selectedPiece = tabcoor;
            this->selectedPieceMoves = this->piece[this->chesstable[tabcoor.y][tabcoor.x]]->ShowMoves(tabcoor.x, tabcoor.y, -1);
            this->filterMoves(this->selectedPiece, this->selectedPieceMoves, -1);
        }
        else
        {
            //if we didn't click on a piece then we check if we clicked on one of the selected pieces moves and if we did we play it
            for (int i = 0; i < this->selectedPieceMoves.size(); i++)
            {
                if (tabcoor == this->selectedPieceMoves[i])
                {
                    //in the case of castling also move the rook
                    if(this->chesstable[this->selectedPiece.y][this->selectedPiece.x] == 'k'){
                        ks = false;
                        if(std::abs(this->selectedPieceMoves[i].x - this->selectedPiece.x) == 2){
                            if(this->selectedPieceMoves[i].x == 1){
                                this->chesstable[7][2] = 'r';
                                this->chesstable[7][0] = ' ';
                            }
                            if(this->selectedPieceMoves[i].x == 5){
                                this->chesstable[7][4] = 'r';
                                this->chesstable[7][7] = ' ';
                            }
                        }
                    }
                    else if(this->chesstable[this->selectedPiece.y][this->selectedPiece.x] == 'r'){
                        if(this->selectedPiece.x == 0){
                            rls = false;
                        }
                        else if(this->selectedPiece.x == 7){
                            rrs = false;
                        }
                    }
                    
                    if(this->chesstable[this->selectedPiece.y][this->selectedPiece.x] == 'p' && this->selectedPieceMoves[i].y == 0){
                        this->chesstable[this->selectedPieceMoves[i].y][this->selectedPieceMoves[i].x] = 'q';
                    }
                    else{
                        this->chesstable[this->selectedPieceMoves[i].y][this->selectedPieceMoves[i].x] = this->chesstable[this->selectedPiece.y][this->selectedPiece.x];
                    }
                    this->chesstable[this->selectedPiece.y][this->selectedPiece.x] = ' ';
                    this->selectedPieceMoves.clear();
                    this->turn = !this->turn;
                }
            }
        }
    }
}


//this works by searching the table for a piece that he has and then checks the possible legal moves of said piece and if there are more than 0 he plays the first one
void Board::SockFish()
{
    if (!this->turn)
    {
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if (this->chesstable[i][j] < 97 && this->chesstable[i][j] != ' ')
                {
                    std::vector<sf::Vector2i> moves = this->piece[this->chesstable[i][j]]->ShowMoves(j, i, 1);
                    this->filterMoves(sf::Vector2i(j, i), moves, 1);
                    if (moves.size() > 0)
                    {
                        if(this->chesstable[i][j] == 'P' && moves[0].y == 7){
                            this->chesstable[moves[0].y][moves[0].x] = 'Q';
                        }
                        else{
                            this->chesstable[moves[0].y][moves[0].x] = this->chesstable[i][j];
                        }
                        this->chesstable[i][j] = ' ';
                        this->turn = !this->turn;
                        return;
                    }
                }
            }
        }
    }
}

void Board::filterMoves(sf::Vector2i p, std::vector<sf::Vector2i> &moves, int sign)
{
    sf::Vector2i kingpos;
    if (sign == 1)
    {
        // going through the moves and removing the move if after it is played the enemy can reach your king
        for (int i = 0; i < moves.size(); i++)
        {
            // memorizing what existed in the old spot so we can revert it later
            char oldspot = this->chesstable[moves[i].y][moves[i].x];

            // playing the move
            this->chesstable[moves[i].y][moves[i].x] = this->chesstable[p.y][p.x];
            this->chesstable[p.y][p.x] = ' ';

            // finding the king position after playing the move
            for (int k = 0; k < 8; k++)
            {
                for (int j = 0; j < 8; j++)
                {
                    if (this->chesstable[k][j] == 'K')
                        kingpos = sf::Vector2i(j, k);
                }
            }

            // finding enemy pieces
            for (int j = 0; j < 8; j++)
            {
                for (int k = 0; k < 8; k++)
                {
                    if (this->chesstable[j][k] >= 97)
                    {
                        // seeing if the enemy pieces can reach your king, and if they can then we can not play the move
                        std::vector<sf::Vector2i> emoves = this->piece[this->chesstable[j][k]]->ShowMoves(k, j, -1);
                        for (int l = 0; l < emoves.size(); l++)
                        {
                            if (emoves[l] == kingpos)
                            {
                                this->chesstable[p.y][p.x] = this->chesstable[moves[i].y][moves[i].x];
                                this->chesstable[moves[i].y][moves[i].x] = oldspot;
                                moves.erase(moves.begin() + i);
                                i--;
                                goto skipchecking;
                            }
                        }
                    }
                }
            }
            this->chesstable[p.y][p.x] = this->chesstable[moves[i].y][moves[i].x];
            this->chesstable[moves[i].y][moves[i].x] = oldspot;
        skipchecking:;
        }
    }
    else
    {
        // going through the moves and removing the move if after it is played the enemy can reach your king
        for (int i = 0; i < moves.size(); i++)
        {
            // memorizing what existed in the old spot so we can revert it later
            char oldspot = this->chesstable[moves[i].y][moves[i].x];

            // playing the move
            this->chesstable[moves[i].y][moves[i].x] = this->chesstable[p.y][p.x];
            this->chesstable[p.y][p.x] = ' ';

            // finding the king position after playing the move
            for (int k = 0; k < 8; k++)
            {
                for (int j = 0; j < 8; j++)
                {
                    if (this->chesstable[k][j] == 'k')
                        kingpos = sf::Vector2i(j, k);
                }
            }

            // finding enemy pieces
            for (int j = 0; j < 8; j++)
            {
                for (int k = 0; k < 8; k++)
                {
                    if (this->chesstable[j][k] < 97 && this->chesstable[j][k] != ' ')
                    {
                        // seeing if the enemy pieces can reach your king, and if they can then we can not play the move
                        std::vector<sf::Vector2i> emoves = this->piece[this->chesstable[j][k]]->ShowMoves(k, j, 1);
                        for (int l = 0; l < emoves.size(); l++)
                        {
                            if (emoves[l] == kingpos)
                            {
                                this->chesstable[p.y][p.x] = this->chesstable[moves[i].y][moves[i].x];
                                this->chesstable[moves[i].y][moves[i].x] = oldspot;
                                moves.erase(moves.begin() + i);
                                i--;
                                goto skipchecking2;
                            }
                        }
                    }
                }
            }
            this->chesstable[p.y][p.x] = this->chesstable[moves[i].y][moves[i].x];
            this->chesstable[moves[i].y][moves[i].x] = oldspot;
        skipchecking2:;
        }
    }
}

Board::Piece::Piece(std::string filename, std::vector<sf::Vector2i> (*ShowMoves)(int x, int y, int sign))
{
    this->ShowMoves = ShowMoves;
    this->tex.loadFromFile(filename);
    this->sprite.setTexture(this->tex);
}

void Board::Piece::Draw(int x, int y, sf::RenderWindow &wind)
{
    this->sprite.setPosition(x, y);
    wind.draw(this->sprite);
}

std::vector<sf::Vector2i> Board::Piece::ShowPawnMoves(int x, int y, int sign)
{
    std::vector<sf::Vector2i> moves;
    if (chesstable[y + sign][x - 1] != ' ')
    {
        int movx = x - 1;
        int movy = y + sign;
        char thepiece = chesstable[movy][movx];
        if (movx >= 0 && movx < 8 && movy >= 0 && movy < 8)
        {
            if (int(thepiece) < 97 && sign == -1)
                moves.push_back(sf::Vector2i(movx, movy));
            else if (int(thepiece) >= 97 && sign == 1)
                moves.push_back(sf::Vector2i(movx, movy));
        }
    }
    if (chesstable[y + sign][x + 1] != ' ')
    {
        int movx = x + 1;
        int movy = y + sign;
        char thepiece = chesstable[movy][movx];
        if (movx >= 0 && movx < 8 && movy >= 0 && movy < 8)
        {
            if (int(thepiece) < 97 && sign == -1)
                moves.push_back(sf::Vector2i(movx, movy));
            else if (int(thepiece) >= 97 && sign == 1)
                moves.push_back(sf::Vector2i(movx, movy));
        }
    }

    for (int ay = 1; ay <= 2; ay++)
    {
        if (ay == 2)
        {
            if ((sign == -1 && y == 6) || (sign == 1 && y == 1))
            {
            }
            else
            {
                break;
            }
        }
        int movx = x;
        int movy = y + ay * sign;
        if (movx >= 0 && movx < 8 && movy >= 0 && movy < 8)
        {
            if (chesstable[movy][movx] == ' ')
            {
                moves.push_back(sf::Vector2i(movx, movy));
            }
            else
            {
                break;
            }
        }
    }
    return moves;
}

std::vector<sf::Vector2i> Board::Piece::ShowRookMoves(int x, int y, int sign)
{
    std::vector<sf::Vector2i> moves;
    for (int xy = 0; xy < 2; xy++)
    {
        for (int i = 1; i >= -1; i -= 2)
        {
            for (int ay = i;; ay += i)
            {
                int movx = x + ay * (bool(xy) ? 1 : 0);
                int movy = y + ay * (bool(xy) ? 0 : 1);
                if (movx >= 0 && movx < 8 && movy >= 0 && movy < 8)
                {
                    if (chesstable[movy][movx] == ' ')
                    {
                        moves.push_back(sf::Vector2i(movx, movy));
                    }
                    else
                    {
                        if (chesstable[movy][movx] < 97 && sign == -1)
                        {
                            moves.push_back(sf::Vector2i(movx, movy));
                        }
                        else if (chesstable[movy][movx] >= 97 && sign == 1)
                        {
                            moves.push_back(sf::Vector2i(movx, movy));
                        }
                        break;
                    }
                }
                else
                {
                    break;
                }
            }
        }
    }
    return moves;
}

std::vector<sf::Vector2i> Board::Piece::ShowKnightMoves(int x, int y, int sign)
{
    std::vector<sf::Vector2i> moves;
    for (int s1 = 1; s1 >= -1; s1 -= 2)
    {
        for (int s2 = 1; s2 >= -1; s2 -= 2)
        {
            for (int sw = 0; sw <= 1; sw++)
            {
                int f = 2;
                int s = 1;
                if (bool(sw))
                {
                    std::swap(f, s);
                }
                f *= s1;
                s *= s2;
                int movx = x + f;
                int movy = y + s;
                if (movx >= 0 && movx < 8 && movy >= 0 && movy < 8)
                {
                    if (chesstable[movy][movx] == ' ')
                    {
                        moves.push_back(sf::Vector2i(movx, movy));
                    }
                    else
                    {
                        if (chesstable[movy][movx] < 97 && sign == -1)
                        {
                            moves.push_back(sf::Vector2i(movx, movy));
                        }
                        else if (chesstable[movy][movx] >= 97 && sign == 1)
                        {
                            moves.push_back(sf::Vector2i(movx, movy));
                        }
                    }
                }
            }
        }
    }
    return moves;
}

std::vector<sf::Vector2i> Board::Piece::ShowKingMoves(int x, int y, int sign)
{
    std::vector<sf::Vector2i> moves;
    for (int s1 = 1; s1 >= -1; s1 -= 1)
    {
        for (int s2 = 1; s2 >= -1; s2 -= 1)
        {
            int movx = x + s1;
            int movy = y + s2;
            if (movx >= 0 && movx < 8 && movy >= 0 && movy < 8)
            {
                if (chesstable[movy][movx] == ' ')
                {
                    moves.push_back(sf::Vector2i(movx, movy));
                }
                else
                {
                    if (chesstable[movy][movx] < 97 && sign == -1)
                    {
                        moves.push_back(sf::Vector2i(movx, movy));
                    }
                    else if (chesstable[movy][movx] >= 97 && sign == 1)
                    {
                        moves.push_back(sf::Vector2i(movx, movy));
                    }
                }
            }
        }
    }
    if(ks){
        if(rls && chesstable[7][1] == ' ' && chesstable[7][2] == ' '){
            moves.push_back(sf::Vector2i(1,7));
        }
        if(rrs && chesstable[7][4] == ' ' && chesstable[7][5] == ' ' && chesstable[7][6] == ' '){
            moves.push_back(sf::Vector2i(5,7));
        }
    }
    return moves;
}

std::vector<sf::Vector2i> Board::Piece::ShowBishopMoves(int x, int y, int sign)
{
    std::vector<sf::Vector2i> moves;
    for (int s1 = 1; s1 >= -1; s1 -= 2)
    {
        for (int s2 = 1; s2 >= -1; s2 -= 2)
        {
            for (int d = 1;; d++)
            {
                int movx = x + s1 * d;
                int movy = y + s2 * d;
                if (movx >= 0 && movx < 8 && movy >= 0 && movy < 8)
                {
                    if (chesstable[movy][movx] == ' ')
                    {
                        moves.push_back(sf::Vector2i(movx, movy));
                    }
                    else
                    {
                        if (chesstable[movy][movx] < 97 && sign == -1)
                        {
                            moves.push_back(sf::Vector2i(movx, movy));
                        }
                        else if (chesstable[movy][movx] >= 97 && sign == 1)
                        {
                            moves.push_back(sf::Vector2i(movx, movy));
                        }
                        break;
                    }
                }
                else
                {
                    break;
                }
            }
        }
    }
    return moves;
}

std::vector<sf::Vector2i> Board::Piece::ShowQueenMoves(int x, int y, int sign)
{
    std::vector<sf::Vector2i> moves;
    for (int s1 = 1; s1 >= -1; s1 -= 1)
    {
        for (int s2 = 1; s2 >= -1; s2 -= 1)
        {
            for (int d = 1;; d++)
            {
                int movx = x + s1 * d;
                int movy = y + s2 * d;
                if (movx >= 0 && movx < 8 && movy >= 0 && movy < 8)
                {
                    if (chesstable[movy][movx] == ' ')
                    {
                        moves.push_back(sf::Vector2i(movx, movy));
                    }
                    else
                    {
                        if (chesstable[movy][movx] < 97 && sign == -1)
                        {
                            moves.push_back(sf::Vector2i(movx, movy));
                        }
                        else if (chesstable[movy][movx] >= 97 && sign == 1)
                        {
                            moves.push_back(sf::Vector2i(movx, movy));
                        }
                        break;
                    }
                }
                else
                {
                    break;
                }
            }
        }
    }
    return moves;
}
#include "pch.h"

//      x ->
//   |
// y V

// 0 1 2
// 3 4 5
// 6 7 8
using TileCode = uint16_t;
using TileIndex = int8_t;
constexpr TileIndex UndefinedTile = -1;
constexpr TileIndex TileSide = 3;

constexpr auto
getpos(const TileIndex index, const TileCode code) -> bool
{
    return code & (1ull << index);
}

enum class Side
{
    Top = 0,
    Bottom,
    Left,
    Right
};
constexpr std::array Sides{ Side::Top, Side::Bottom, Side::Left, Side::Right };

constexpr auto
sideName(const Side side) -> std::string_view
{
    switch (side)
    {
    case Side::Top:
        return "Top";
    case Side::Bottom:
        return "Bottom";
    case Side::Left:
        return "Left";
    case Side::Right:
        return "Right";
    default:
        throw std::runtime_error("unexpected case");
    }
}

constexpr auto
opposite(const Side side) -> Side
{
    switch (side)
    {
    case Side::Top:
        return Side::Bottom;
    case Side::Bottom:
        return Side::Top;
    case Side::Left:
        return Side::Right;
    case Side::Right:
        return Side::Left;
    default:
        throw std::runtime_error("unexpected case");
    }
}

constexpr auto
indicesForSide(const Side side) -> std::array<TileIndex, TileSide>
{
    switch (side)
    {
    case Side::Top:
        return { 0, 1, 2 };
    case Side::Bottom:
        return { 6, 7, 8 };
    case Side::Left:
        return { 0, 3, 6 };
    case Side::Right:
        return { 2, 5, 8 };
    default:
        throw std::runtime_error("unexpected case");
    }
}

constexpr auto
compatible(const TileCode a, const TileCode b, const Side side) -> bool
{
    const auto aIndices = indicesForSide(side);
    const auto bIndices = indicesForSide(opposite(side));
    for (TileIndex i = 0; i < TileSide; ++i)
        if (getpos(aIndices[i], a) != getpos(bIndices[i], b))
            return false;
    return true;
}

// _x_
// ___
// ___
constexpr TileCode Tile1 = 0b000000010;

// ___
// ___
// _x_
constexpr TileCode Tile2 = 0b010000000;

// ___
// _xx
// _x_
constexpr TileCode Tile3 = 0b010110000;

// _x_
// xx_
// _x_
constexpr TileCode Tile4 = 0b010011010;

static_assert(compatible(Tile1, Tile2, Side::Top));
static_assert(compatible(Tile1, Tile2, Side::Bottom));
static_assert(compatible(Tile1, Tile2, Side::Right));
static_assert(compatible(Tile1, Tile2, Side::Left));

static_assert(!compatible(Tile2, Tile3, Side::Left));
static_assert(compatible(Tile2, Tile3, Side::Right));
static_assert(!compatible(Tile2, Tile3, Side::Top));
static_assert(!compatible(Tile2, Tile3, Side::Bottom));

constexpr std::array Tiles
{
    Tile1, Tile2, Tile3, Tile4
};
constexpr size_t NumTiles = Tiles.size();

using CompatibilityMask = uint8_t;
static_assert(sizeof(CompatibilityMask) * 8 >= NumTiles);
constexpr auto
compatibleIndices(const size_t index, const Side side) -> CompatibilityMask
{
    CompatibilityMask compatibilityMask{};
    size_t currentIndex{};
    for (size_t otherIndex = 0; otherIndex < NumTiles; ++otherIndex)
        if (compatible(Tiles.at(index), Tiles.at(otherIndex), side))
            compatibilityMask |= 1ull << otherIndex;
    return compatibilityMask;
}

constexpr auto
compatibleSides(const size_t index) -> std::array<CompatibilityMask, Sides.size()>
{
    std::array<CompatibilityMask, Sides.size()> ret{};
    size_t idx{};
    for (const auto side : Sides)
        ret[idx++] = compatibleIndices(index, side);
    return ret;
}

using CompatibilityMap = std::array<std::array<CompatibilityMask, Sides.size()>, NumTiles>;
constexpr auto
generateCompatibilityMap() -> CompatibilityMap
{
    CompatibilityMap ret{};
    for (size_t tileIdx = 0; tileIdx < NumTiles; ++tileIdx)
        ret[tileIdx] = compatibleSides(tileIdx);
    return ret;
}

constexpr auto compatibilityMap = generateCompatibilityMap();

constexpr bool
solvable()
{
    for (const auto tileData : compatibilityMap)
        for (const auto sideData : tileData)
            if (sideData == 0)
                return false;
    return true;
}

static_assert(solvable());

void printCompatiblityMap()
{
    size_t masterTileIndex{};
    for (const auto sides : compatibilityMap)
    {
        std::cout << "Master tile: " << masterTileIndex++ << '\n';
        size_t sideIndex{};
        for (const auto side : sides)
        {
            std::cout << "\tSide: " << sideName(Sides[sideIndex++])
                << " compatible: " << std::bitset<NumTiles>(side) << '\n';
        }
    }
}

class World
{
public:
    World(const size_t width, const size_t height) :
        _width(width),
        _height(height),
        _data(width * height)
    {}

    TileIndex& at(size_t col, size_t row)
    {
        return _data.at(row * _width + col);
    }

    size_t width() const noexcept { return _width; }
    size_t height() const noexcept { return _height; }

private:
    size_t _width;
    size_t _height;
    std::vector<TileIndex> _data;
};

constexpr std::array Directions{
    std::make_pair(-1, 0),
    std::make_pair(1, 0),
    std::make_pair(0, 1),
    std::make_pair(0, -1)
};

auto
tileOptions(const World& world, const size_t col, const size_t row) -> CompatibilityMask
{
    CompatibilityMask resultMask = std::numeric_limits<CompatibilityMask>::max();
    for (const auto [dx, dy] : Directions)
    {

    }
}

int main()
{
    printCompatiblityMap();
}

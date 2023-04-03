#pragma warning( disable : 4251 )
#pragma warning( disable : 4099 )

#include "legacystrike.hxx"

int main() {
    legacystrike::sqlConnection::sqlConnect();
    legacystrike::botHandler::handleBot();
}

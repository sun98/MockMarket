#include "order.h"
#include "market.h"
#include "client.h"

int main() {
    Market market;
    Client client("./data/inserts_test.csv", "./data/cancels_test.csv", true);
    // Client client("./data/inserts.csv", "./data/cancels.csv", false);

    while (client.hasOrder()) {
        Order &order = client.sendOrder();
        auto new_trades = market.handleOrder(order);
        market.notifyClient(client, *new_trades);
    }
    market.showStatus(STATUS_MIN);

    return 0;
}
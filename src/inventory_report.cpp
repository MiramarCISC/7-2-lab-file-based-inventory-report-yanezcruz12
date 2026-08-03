#include "inventory_report.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>

using namespace std;

bool isValidQuantity(int quantity) {
    // Quantity must be 0 or greater.
    return quantity >= 0;
}

bool isValidPrice(double price) {
    // Price must be 0 or greater
    return price >= 0.0;
}

double calculateItemValue(const InventoryItem& item) {
    // Return 0.0 if the item contains invalid data.
    if (!isValidQuantity(item.quantity) || !isValidPrice(item.price)) {
        return 0.0;
    }

    // Calculates the total value of the item.
    return item.quantity * item.price;
}

int readInventoryFile(string filename, InventoryItem items[], int maxItems) {
 // Check that the array and maximum size are valid.
    if (items == nullptr || maxItems <= 0) {
        return 0;
    }

    // Opens the input file.
    ifstream in(filename);

    // Returns 0 if the file could not be opened.
    if (!in.is_open()) {
        return 0;
    }

    int count = 0;
    InventoryItem item;

    // Read records until the file ends or the array is full.
    while (count < maxItems &&
           in >> item.sku
              >> item.name
              >> item.quantity
              >> item.price) {

        // Stores only records with valid quantities and prices.
        if (isValidQuantity(item.quantity) &&
            isValidPrice(item.price)) {
            items[count] = item;
            count++;
        }
    }

    // Close the input file.
    in.close();

    return count;
}

bool writeInventoryReport(string filename, const InventoryItem items[], int count) {
// Check that the array and count are valid.
    if (items == nullptr || count < 0) {
        return false;
    }

    // Opens the output file.
    ofstream out(filename);

    // Returns false if the file could not be opened.
    if (!out.is_open()) {
        return false;
    }

    // Formats decimal values to two places.
    out << fixed << setprecision(2);

    out << "Inventory Report" << endl;
    out << "SKU Name Quantity Price Value" << endl;

    // Writes each inventory item to the report.
    for (int i = 0; i < count; i++) {
        out << items[i].sku << " "
            << items[i].name << " "
            << items[i].quantity << " "
            << items[i].price << " "
            << calculateItemValue(items[i])
            << endl;
    }

    // Write the total value of all inventory items.
    out << "Total inventory value: "
        << calculateTotalInventoryValue(items, count)
        << endl;

    // Close the output file.
    out.close();

    return true;
}


double calculateTotalInventoryValue(const InventoryItem items[], int count) {
    // Checks that the array and count are valid.
    if (items == nullptr || count <= 0) {
        return 0.0;
    }

    double total = 0.0;

    // Adds the value of every item.
    for (int i = 0; i < count; i++) {
        total += calculateItemValue(items[i]);
    }

    return total;
}

int findItemBySku(const InventoryItem items[], int count, string sku) {
    // Checks that the array and count are valid.
    if (items == nullptr || count <= 0) {
        return -1;
    }

    // Search for a matching SKU.
    for (int i = 0; i < count; i++) {
        if (items[i].sku == sku) {
            return i;
        }
    }

    return -1;
}

int findHighestValueItemIndex(const InventoryItem items[], int count) {
    // Checks that the array and count are valid.
    if (items == nullptr || count <= 0) {
        return -1;
    }

    // Start by treating the first item as the highest.
    int highestIndex = 0;

    // Next compare the remaining items.
    for (int i = 1; i < count; i++) {
        if (calculateItemValue(items[i]) >
            calculateItemValue(items[highestIndex])) {
            highestIndex = i;
        }
    }

    return highestIndex;
}

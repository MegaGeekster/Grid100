#ifndef CELL_H
#define CELL_H
#include <QPushButton>
#include "Styles.h"

struct Cell
{
    Styles::StyleType styleType{Styles::StyleType::EMPTY_CELL};
    QPushButton *button{nullptr};
    uint32_t number{0U};
};

#endif // CELL_H

#include "LJW/IPlayerInterAction.h"
#include "KJW/ItemType.h"

void IIPlayerInterAction::UseItem ( EItemType ItemType )
{
	switch (ItemType)
    {
        case EItemType::THROW:
            break;
        case EItemType::USING:
            break;
        case EItemType::TRAP:
            break;
        case EItemType::MAX:
            break;
        default:
            break;
    }
}

void IIPlayerInterAction::ThrowItem ( EThrowType ThrowType )
{
     switch (ThrowType)
    {
        case EThrowType::STONE:
            break;
        case EThrowType::NET:
            break;
        case EThrowType::MAX:
            break;
        default:
            break;
    }
}

void IIPlayerInterAction::BuffItem ( EUSINGType BuffType )
{
    
}

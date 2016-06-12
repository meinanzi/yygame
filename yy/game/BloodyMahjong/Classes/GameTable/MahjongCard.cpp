#include "MahjongCard.h"
#include "GameManager.h"

namespace XZMJ
{

	MahjongCard::MahjongCard(void)
	{
	}


	MahjongCard::~MahjongCard(void)
	{
	}

	bool MahjongCard::init(mahjongCreateType type, sitDir sitNo, INT number)
	{
		if (! Card::init(type, sitNo, number))
		{
			return false;
		}
		_sum = number;
		// 增加麻将界面显示
		_type = type;
		_number = number % 10;
		_sitNo = sitNo;

		_color = mahjongColor(number / 10);
		std::string head = "";
		switch (type)
		{
		case XZMJ::mahjongCreateType::ZUO_LI:
			head = "liZuo";
			break;
		case XZMJ::mahjongCreateType::YOU_LI:
			head = "liYou";
			break;
		case XZMJ::mahjongCreateType::QIAN_LI:
			head = "liNorthxiao";
			break;
		case XZMJ::mahjongCreateType::DI_BEI_HENG:
			head = "DaoBeiEW";
			break;
		case XZMJ::mahjongCreateType::DI_BEI_SHU:
			head = "DaoBeiNS";
			break;
		case XZMJ::mahjongCreateType::DI_ZHENG_SOUTH:
			head = "ChuSouth";
			break;
		case XZMJ::mahjongCreateType::DI_ZHENG_NORTH:
			head = "ChuNorth";
			break;
		case XZMJ::mahjongCreateType::DI_ZHENG_EAST:
			head = "ChuEast";
			break;
		case XZMJ::mahjongCreateType::DI_ZHENG_WEST:
			head = "ChuWest";
			break;
		case XZMJ::mahjongCreateType::ZHENG_LI:
			head = "ZhengDa";
			break;
		case XZMJ::mahjongCreateType::ZHENG_DAO:
			head = "DaoDa";
			break;
		case XZMJ::mahjongCreateType::HU_BEI_DAO:
			head = "huCard";
			break;
		default:
			break;
		}

		switch (_color)
		{
		case XZMJ::mahjongColor::WAN:
			head += "Wan";
			head += StringUtils::format("%d", _number);
			break;
		case XZMJ::mahjongColor::TIAO:
			head += "Tiao";
			head += StringUtils::format("%d", _number);
			break;
		case XZMJ::mahjongColor::TONG:
			head += "Tong";
			head += StringUtils::format("%d", _number);
			break;
		default:
			break;
		}

		_uiSp = PLIST_MAKE_A_NODE(head);
		_uiSp->setVisible(true);
		_uiSp->setPosition(-100, -100);
		_size = _uiSp->getContentSize();
		return true;
	}

}
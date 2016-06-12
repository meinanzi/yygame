#include "ZYHZ_MahjongCard.h"
#include "ZYHZ_GameManager.h"

namespace ZYHZ
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
		float scale = getScaleXX();
		_color = mahjongColor(number / 10);
		std::string head = "";
		switch (type)
		{
		case ZYHZ::mahjongCreateType::ZUO_LI:
			head = "liZuo";
			break;
		case ZYHZ::mahjongCreateType::YOU_LI:
			head = "liYou";
			break;
		case ZYHZ::mahjongCreateType::QIAN_LI:
			head = "liNorthxiao";
			break;
		case ZYHZ::mahjongCreateType::DI_BEI_HENG:
			head = "DaoBeiEW";
			break;
		case ZYHZ::mahjongCreateType::DI_BEI_SHU:
			head = "DaoBeiNS";
			break;
		case ZYHZ::mahjongCreateType::DI_ZHENG_SOUTH:
			head = "ChuSouth";
			break;
		case ZYHZ::mahjongCreateType::DI_ZHENG_NORTH:
			head = "ChuNorth";
			break;
		case ZYHZ::mahjongCreateType::DI_ZHENG_EAST:
			head = "ChuEast";
			break;
		case ZYHZ::mahjongCreateType::DI_ZHENG_WEST:
			head = "ChuWest";
			break;
		case ZYHZ::mahjongCreateType::ZHENG_LI:
			head = "ZhengDa";
			break;
		case ZYHZ::mahjongCreateType::ZHENG_DAO:
			head = "DaoDa";
			break;
		case ZYHZ::mahjongCreateType::HU_BEI_DAO:
			head = "huCard";
			break;
		default:
			break;
		}

		if (number == CMjEnum::MJ_TYPE_ZHONG)  // 红中
		{
			head += "Zhong";
		}

		switch (_color)
		{
		case ZYHZ::mahjongColor::WAN:
			head += "Wan";
			head += StringUtils::format("%d", _number);
			break;
		case ZYHZ::mahjongColor::TIAO:
			head += "Tiao";
			head += StringUtils::format("%d", _number);
			break;
		case ZYHZ::mahjongColor::TONG:
			head += "Tong";
			head += StringUtils::format("%d", _number);
			break;
		default:
			break;
		}

		_uiSp = PLIST_MAKE_A_NODE(head);
		_uiSp->setVisible(true);
		_uiSp->setPosition(-100, -100);
		_uiSp->setScale(scale);
		if (_type == mahjongCreateType::ZHENG_LI)
		{
			_uiSp->setScale(scale + 0.16f, scale);
		}

		auto newSize = _uiSp->getContentSize() * scale;
		if (_type == mahjongCreateType::ZHENG_LI)
		{
			newSize = Size(_uiSp->getContentSize().width * (scale + 0.16f), _uiSp->getContentSize().height * scale);
		}
		_uiSp->setContentSize(newSize);
		_size = newSize;
		return true;
	}

}
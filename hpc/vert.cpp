#include "vert.h"
bool HE_vert::set_cutchoice(int cutchoiceID)
{
	if (cutchoiceID>=cutedpoints.size())
	{
		return false;
	}
	for (auto iter = unavailiable.begin(); iter != unavailiable.end(); iter++)
	{
		if (cutchoiceID == *iter)
			return false;
	}
	cutchoice.push_back(cutchoiceID);
	last_unavailiable_size.push_back(unavailiable.size());
	for (int t1 = 0; t1 < cutedpoints.size(); t1++)//¶ÔÓÚÃ¿ÖÖÇÐžî·œÊœ
	{
		for (int t2 = 0; t2 < cutedpoints.at(t1).size(); t2++)//¶ÔÓÚÇÐžî·œÊœÖÐµÄÃ¿Ò»Ìõ±ß
		{
			for (int t3 = 0; t3 < cutedpoints.at(cutchoiceID).size(); t3++)//¶ÔÓÚÑ¡ÔñµÄÇÐžî·œÊœµÄÃ¿Ò»Ìõ±ß
			{
				if (cutedpoints.at(t1).at(t2) == cutedpoints.at(cutchoiceID).at(t3))//Èç¹ûÒÑŸ­ÓÐÏàÍ¬µÄ±ßÁË
				{
					//	printf("id %d,unavailable size %d,thread%d\n",this->id(), unavailiable.size(),omp_get_thread_num());
					unavailiable.push_back(t1);
					break;
				}
			}
		}
	}
	//printf("*********************************************\n");
	return true;
};
bool HE_vert::reset_cutchoice(){

for (int w = unavailiable.size(); w > last_unavailiable_size.back(); w--)
	{
		unavailiable.pop_back();
	}
	last_unavailiable_size.pop_back();
	return true;
}

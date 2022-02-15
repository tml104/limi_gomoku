#include "AIComponent.h"
#include <algorithm>
#include <memory.h>
#include <assert.h>
/* len(piece_array) == 9, piece_array[4] == start_piece
* 0:black, 1:white, 2:none , -1:out of bound
*/
const int PIECE_ARRAY_N = 9;
const int PIECE_ARRAY_START = 4;
ll judgeDragon_B(int piece_array[], int dragonState[2][2][6])
{
	ll ans = 0;
	ll flag = 1;
	//black dragon
	if (piece_array[PIECE_ARRAY_START] == 0)
	{
		flag = 1;
		//turn white to -1
		for (int i=0;i< PIECE_ARRAY_N;i++)
		{
			if (piece_array[i] == 1)
			{
				piece_array[i] = -1;
			}
			
		}
	}
	// white dragon
	else if (piece_array[PIECE_ARRAY_START] == 1)
	{
		flag = -1;
		//turn black to -1
		for (int i = 0; i < PIECE_ARRAY_N; i++)
		{
			if (piece_array[i] == 0)
			{
				piece_array[i] = -1;
			}
			//turn white to 0 for judge
			else if (piece_array[i] == 1) piece_array[i] = 0;
		}
	}
	else //illegal
	{
		flag = 0;
		return 0;
	}

	//now, 0: vaild piece, -1:invaild place, 2:none
	//judge functions
	auto huo_five = [&]() {
		bool flag = true;
		for (int i = PIECE_ARRAY_START; i <= PIECE_ARRAY_START + 4; i++)
		{
			if (piece_array[i] != 0)
			{
				flag = false;
				break;
			}
		}

		//if (flag) return HUO_DRAGON_SCORE[5];
		//return 0ll;
		return flag;
	};

	auto huo_four = [&]() {
		bool flag = true;
		for (int i = PIECE_ARRAY_START; i <= PIECE_ARRAY_START + 3; i++)
		{
			if (piece_array[i] != 0)
			{
				flag = false;
				break;
			}
		}

		if (piece_array[PIECE_ARRAY_START - 1] != 2) flag = false;
		if (piece_array[PIECE_ARRAY_START + 4] != 2) flag = false;

		//if (flag) return HUO_DRAGON_SCORE[4];
		//return 0ll;
		return flag;
	};

	auto chong_four = [&](bool is_huo_four) {
		if (is_huo_four) return false;

		if (piece_array[PIECE_ARRAY_START - 1] == 0) return false;
		//bool flag = true;

		if (
			piece_array[PIECE_ARRAY_START] == 0 &&
			piece_array[PIECE_ARRAY_START + 1] == 0 &&
			piece_array[PIECE_ARRAY_START + 2] == 0 &&
			piece_array[PIECE_ARRAY_START + 3] == 0
			)
		{
			if (piece_array[PIECE_ARRAY_START - 1] == 2 || piece_array[PIECE_ARRAY_START + 4] == 2)
			{
				return true;
			}
		}

		//else
		bool flag = true;
		int cnt = 0;
		for (int i = PIECE_ARRAY_START; i <= PIECE_ARRAY_START + 4; i++)
		{
			if (piece_array[i] == -1)
			{
				flag = false;
				break;
			}
			else if (piece_array[i] == 0)
			{
				cnt++;
			}
		}

		if (flag && cnt == 4) return true;
		return false;
	};

	auto huo_three = [&](bool is_huo_four) {
		if (is_huo_four) return false;

		if (
			piece_array[PIECE_ARRAY_START] == 0 &&
			piece_array[PIECE_ARRAY_START + 1] == 0 &&
			piece_array[PIECE_ARRAY_START + 2] == 0 &&
			piece_array[PIECE_ARRAY_START + 3] == 2 &&
			piece_array[PIECE_ARRAY_START - 1] == 2
			)
		{
			if (piece_array[PIECE_ARRAY_START - 2] == 0 || piece_array[PIECE_ARRAY_START + 4] == 0) return false;
			if (!(piece_array[PIECE_ARRAY_START - 2] == -1 && piece_array[PIECE_ARRAY_START + 4] == -1))
			{
				return true;
			}
		}

		return false;
	};

	auto chong_three = [&](bool is_huo_three, bool is_huo_four, bool is_chong_four) {
		if (is_huo_three) return false;
		if (is_huo_four) return false;
		if (is_chong_four) return false;

		if (piece_array[PIECE_ARRAY_START - 1] == 0) return false;

		// ~ooo~
		if (
			piece_array[PIECE_ARRAY_START] == 0 &&
			piece_array[PIECE_ARRAY_START + 1] == 0 &&
			piece_array[PIECE_ARRAY_START + 2] == 0 &&
			piece_array[PIECE_ARRAY_START + 3] == 2 &&
			piece_array[PIECE_ARRAY_START - 1] == 2
			)
		{
			//no x~ooo~x
			if (piece_array[PIECE_ARRAY_START - 2] == -1 && piece_array[PIECE_ARRAY_START + 4] == -1)
			{
				return true;
			}
		}

		// ooo
		if (piece_array[PIECE_ARRAY_START] == 0 &&
			piece_array[PIECE_ARRAY_START + 1] == 0 &&
			piece_array[PIECE_ARRAY_START + 2] == 0
			)
		{
			// xooo~~
			if (piece_array[PIECE_ARRAY_START - 1] == -1 &&
				piece_array[PIECE_ARRAY_START + 3] == 2 &&
				piece_array[PIECE_ARRAY_START + 4] == 2
				)
			{
				return true;
			}

			// ~~ooox
			if (
				piece_array[PIECE_ARRAY_START + 3] == -1 &&
				piece_array[PIECE_ARRAY_START - 1] == 2 &&
				piece_array[PIECE_ARRAY_START - 2] == 2
				)
			{
				return true;
			}
		}

		if (
			(
				(
					piece_array[PIECE_ARRAY_START] == 0 &&
					piece_array[PIECE_ARRAY_START + 1] == 0 &&
					piece_array[PIECE_ARRAY_START + 2] == 2 &&
					piece_array[PIECE_ARRAY_START + 3] == 0
					) ||
				(
					piece_array[PIECE_ARRAY_START] == 0 &&
					piece_array[PIECE_ARRAY_START + 1] == 2 &&
					piece_array[PIECE_ARRAY_START + 2] == 0 &&
					piece_array[PIECE_ARRAY_START + 3] == 0
					)
				) &&
				(
					(piece_array[PIECE_ARRAY_START - 1] == 2 && (piece_array[PIECE_ARRAY_START + 4] != 0)) ||
					(piece_array[PIECE_ARRAY_START - 1] == -1 && (piece_array[PIECE_ARRAY_START + 4] == 2))
				)

			)
		{
			return true;
		}

		return false;
	};

	auto huo_two = [&]() {

		if (
			piece_array[PIECE_ARRAY_START] == 0 &&
			piece_array[PIECE_ARRAY_START + 1] == 0 &&
			piece_array[PIECE_ARRAY_START - 1] == 2 &&
			piece_array[PIECE_ARRAY_START + 2] == 2 
		)
		{
			if (piece_array[PIECE_ARRAY_START - 2]==2 || piece_array[PIECE_ARRAY_START + 3] == 2)
			{
				return true;
			}
		}

		return false;
	};

	auto chong_two = [&](bool is_huo_two) {

		if (is_huo_two) return false;
		if (piece_array[PIECE_ARRAY_START - 1] == 0) return false;

		//0
		if (
			piece_array[PIECE_ARRAY_START] == 0 &&
			piece_array[PIECE_ARRAY_START + 1] == 0 &&
			piece_array[PIECE_ARRAY_START + 2] == 2 &&
			piece_array[PIECE_ARRAY_START + 3] == 2 &&
			piece_array[PIECE_ARRAY_START - 1] == -1
			)
		{
			return true;
		}

		if (
			piece_array[PIECE_ARRAY_START] == 0 &&
			piece_array[PIECE_ARRAY_START + 1] == 0 &&
			piece_array[PIECE_ARRAY_START + 2] == -1 &&
			piece_array[PIECE_ARRAY_START - 1] == 2 &&
			piece_array[PIECE_ARRAY_START - 2] == 2
			)
		{
			return true;
		}

		//1
		if (
			piece_array[PIECE_ARRAY_START] == 0 &&
			piece_array[PIECE_ARRAY_START + 2] == 0 &&
			piece_array[PIECE_ARRAY_START + 1] == 2
		)
		{
			if (piece_array[PIECE_ARRAY_START - 1] == 2 && piece_array[PIECE_ARRAY_START + 3] != 0)
			{
				return true;
			}

			if (piece_array[PIECE_ARRAY_START - 1] == -1 && piece_array[PIECE_ARRAY_START + 3] == 2)
			{
				return true;
			}


		}

		return false;
	};

	auto huo_one = [&]() {
		return (piece_array[PIECE_ARRAY_START] == 0 && piece_array[PIECE_ARRAY_START - 1] == 2 && piece_array[PIECE_ARRAY_START + 1] == 2);
	};

	auto chong_one = [&]() {
		return (piece_array[PIECE_ARRAY_START] == 0 && piece_array[PIECE_ARRAY_START - 1] == -1 && piece_array[PIECE_ARRAY_START + 1] == 2) || 
			(piece_array[PIECE_ARRAY_START] == 0 && piece_array[PIECE_ARRAY_START - 1] == 2 && piece_array[PIECE_ARRAY_START + 1] == -1);
	};


	//logic
	int flag2 = (flag == 1) ? 0 : 1;

	if (huo_five())
	{
		ans = HUO_DRAGON_SCORE[5];
		if (dragonState!=nullptr)
		{
			++dragonState[flag2][0][5];
		}
	}
	else
	{
		//4
		bool is_huo_four = huo_four();
		bool is_chong_four = chong_four(is_huo_four);
		if (is_huo_four) { ans += HUO_DRAGON_SCORE[4]; 
			if (dragonState != nullptr)
			{
				++dragonState[flag2][0][4];
			}
		}
		if (is_chong_four) { ans += CHONG_DRAGON_SCORE[4]; 
			if (dragonState != nullptr)
			{
				++dragonState[flag2][1][4];
			}
		}

		//3
		bool is_huo_three = huo_three(is_huo_four);
		bool is_chong_three = chong_three(is_huo_three, is_huo_four, is_chong_four);
		if (is_huo_three) { ans += HUO_DRAGON_SCORE[3]; 
			if (dragonState != nullptr)
			{
				++dragonState[flag2][0][3];
			}
		}
		if (is_chong_three) { ans += CHONG_DRAGON_SCORE[3]; 
			if (dragonState != nullptr)
			{
				++dragonState[flag2][1][3];
			}
		}

		//2
		bool is_huo_two = huo_two();
		bool is_chong_two = chong_two(is_huo_two);
		if (is_huo_two) { ans += HUO_DRAGON_SCORE[2]; 
			if (dragonState != nullptr)
			{
				++dragonState[flag2][0][2];
			}
		}
		if (is_chong_two) { ans += CHONG_DRAGON_SCORE[2]; 
			if (dragonState != nullptr)
			{
				++dragonState[flag2][1][2];
			}
		}
		
		//1
		bool is_huo_one = huo_one();
		bool is_chong_one = chong_one();
		if (is_huo_one) {ans += HUO_DRAGON_SCORE[1];
			if (dragonState != nullptr)
			{
				++dragonState[flag2][0][1];
			}
		}
		if (is_chong_one) { ans += CHONG_DRAGON_SCORE[1]; 
		
			if (dragonState != nullptr)
			{
				++dragonState[flag2][1][1];
			}
		}
	}
	
	return ans * flag;

}


/*
 L -> R
*/
ll judgeDragon_A_0(gb_pt tmpGameBoard, int x, int y, int dragonState[2][2][6])
{
	int piece_array[PIECE_ARRAY_N];

	for (int i=-PIECE_ARRAY_START;i<=PIECE_ARRAY_START;i++)
	{
		piece_array[i + PIECE_ARRAY_START] = tmpGameBoard->getWhoPiece(x, y + i);
	}

	return judgeDragon_B(piece_array, dragonState);
}


/*
 LU -> DR
*/
ll judgeDragon_A_1(gb_pt tmpGameBoard, int x, int y, int dragonState[2][2][6])
{
	int piece_array[PIECE_ARRAY_N];

	for (int i = -PIECE_ARRAY_START; i <= PIECE_ARRAY_START; i++)
	{
		piece_array[i + PIECE_ARRAY_START] = tmpGameBoard->getWhoPiece(x + i, y + i);
	}

	return judgeDragon_B(piece_array, dragonState);
}

/*
 U -> R
*/
ll judgeDragon_A_2(gb_pt tmpGameBoard, int x, int y, int dragonState[2][2][6])
{
	int piece_array[PIECE_ARRAY_N];

	for (int i = -PIECE_ARRAY_START; i <= PIECE_ARRAY_START; i++)
	{
		piece_array[i + PIECE_ARRAY_START] = tmpGameBoard->getWhoPiece(x + i, y);
	}

	return judgeDragon_B(piece_array, dragonState);
}

/*
 LD  -> UR
*/
ll judgeDragon_A_3(gb_pt tmpGameBoard, int x, int y, int dragonState[2][2][6])
{
	int piece_array[PIECE_ARRAY_N];

	for (int i = -PIECE_ARRAY_START; i <= PIECE_ARRAY_START; i++)
	{
		piece_array[i + PIECE_ARRAY_START] = tmpGameBoard->getWhoPiece(x - i, y + i);
	}

	return judgeDragon_B(piece_array, dragonState);
}


const int JUDGE_DIS = 4;
/*
Return the 4 direction dragon val sum at (x,y)
*/
ll judgeDragon_A(gb_pt tmpGameBoard, int x, int y, int mask, int dragonState[2][2][6])
{
	auto judgeDragon_AA_0 = [&]() {
		ll ans = 0;
		for (int i = -JUDGE_DIS; i <= JUDGE_DIS; i++)
		{
			ans += judgeDragon_A_0(tmpGameBoard, x, y + i, dragonState);
		}
		return ans;
	};

	auto judgeDragon_AA_1 = [&]() {
		ll ans = 0;
		for (int i = -JUDGE_DIS; i <= JUDGE_DIS; i++)
		{
			ans += judgeDragon_A_1(tmpGameBoard, x+i, y + i, dragonState);
		}
		return ans;

	};

	auto judgeDragon_AA_2 = [&]() {
		ll ans = 0;
		for (int i = -JUDGE_DIS; i <= JUDGE_DIS; i++)
		{
			ans += judgeDragon_A_2(tmpGameBoard, x+i, y, dragonState);
		}
		return ans;

	};

	auto judgeDragon_AA_3 = [&]() {
		ll ans = 0;
		for (int i = -JUDGE_DIS; i <= JUDGE_DIS; i++)
		{
			ans += judgeDragon_A_3(tmpGameBoard, x-i, y + i, dragonState);
		}
		return ans;
	};


	//return judgeDragon_A_0(tmpGameBoard, x, y) +
	//	judgeDragon_A_1(tmpGameBoard, x, y) +
	//	judgeDragon_A_2(tmpGameBoard, x, y) +
	//	judgeDragon_A_3(tmpGameBoard, x, y);

	ll ans = 0;

	if ((mask) & 1)
	{
		ans += judgeDragon_AA_0();
	}

	if ((mask>>1) & 1)
	{
		ans += judgeDragon_AA_1();
	}

	if ((mask>>2) & 1)
	{
		ans += judgeDragon_AA_2();
	}

	if ((mask>>3) & 1)
	{
		ans += judgeDragon_AA_3();
	}

	return ans;
}


st_pt Search_B(
	int depth,
	int search_id,
	StateMap &stateMap,
	gb_pt tmpGameBoard,
	st_pt fatherState,
	ll alpha,
	ll beta,
	ll limitDepth,
	int newx,
	int newy,
	int is_min,
	int fast_search,
	Static& stc
)
{
	++stc.search_cnt;
	if (fast_search > 0)
	{
		++stc.fast_search_cnt;

	}
	
	int limit_width_cnt = 0;
	int limit_width = std::max((LIMIT_WIDTH==13?4:6), LIMIT_WIDTH - ((depth - 1) / 2) * 2);
	//int limit_width = LIMIT_WIDTH;
	//if (fast_search >0) limit_width = 2;
	
	//Get State
	auto it = stateMap.find(tmpGameBoard->hashVal);
	st_pt nowState;
	bool state_searched = false;
	if ( it == stateMap.end() ) //no find
	{
		//copy
		nowState = std::make_shared<State>(*fatherState);

		//update hashval
		nowState->hashVal = tmpGameBoard->hashVal;

		//update State score
		nowState->updateEstimatedScore(tmpGameBoard, newx, newy);
		
		//update Next Step Map
		nowState->updateNextStepMap(tmpGameBoard, newx, newy);


		//add this state to stateMap
		stateMap[tmpGameBoard->hashVal] = { nowState, search_id};
	}
	else //already exist
	{
		++stc.already_exist_cnt;

		nowState = (it->second).first;
		int search_id2 = (it->second).second;

		if (search_id2 == search_id)
		{
			++stc.state_searched_cnt;

			state_searched = true;
		}
		else
		{
			//update search id
			(it->second).second = search_id;
		}
	}

	//bruteforceCheckDragon(nowState->dragonState, tmpGameBoard);

	//bound condition

	auto bound_cond_lambda = [&]()
	{
		nowState->actualScore = nowState->estimatedScore;
		nowState->ans = { -1,-1 };
		nowState->ansHashVal = 0;
		return nowState;
	};

	if (std::abs(nowState->estimatedScore)>=WIN_THRESHOLD) //limitDepth or get 5
	{
		return bound_cond_lambda();
	}

	if (std::abs(nowState->estimatedScore) >= PREWIN_THRESHOLD && depth>1)
	{
		stc.pre_ret++;
		return bound_cond_lambda();
	}

	if (depth == limitDepth)
	{
		if (fast_search!=0)
		{
			return bound_cond_lambda();
		}
		else if(fast_search == 0) //fast_search = 0
		{
			if (
				//(is_min && ((nowState->estimatedScore) <= -FAST_THRESHOLD)) ||
				//(!is_min && ((nowState->estimatedScore) >= FAST_THRESHOLD))
				( std::abs(nowState->estimatedScore) >= FAST_THRESHOLD ) 
			)
			{
				if ((nowState->estimatedScore) <= -FAST_THRESHOLD)
				{
					fast_search = 2;
				}
				else fast_search = 1;

				//fast_search = 1 + is_min; // 1: black attack, 2: white attack
				limitDepth += 8;
				limit_width = 2;
			}
			else
			{
				return bound_cond_lambda();
			}
		}
		else if (fast_search == -1)
		{
			return bound_cond_lambda();
		}
	}


	//alpha-beta search
	if (state_searched) //searched
	{
		return
			nowState;

	}

	//init nextStepVector
	//std::vector<std::pair<std::pair<int, int>,ll>> nextStepVec;
	if (nowState->nextStepVec.size() == 0 && fast_search<=0)
	{
		for (auto& v : nowState->nextStepMap[is_min])
		{
			nowState->nextStepVec.emplace_back(v.first,(v.second));
		}

		for (auto& v : nowState->nextStepMap[is_min^1])
		{
			nowState->nextStepVec.emplace_back(v.first, (v.second));
		}

		auto cmp = [&](const std::pair<std::pair<int, int>, ll> a,
			const std::pair<std::pair<int, int>, ll> b)
		{
			//return ((is_min) ? (a.second<b.second) : (a.second>b.second));
			return (a.second) > (b.second);
		};

		std::sort(
			nowState->nextStepVec.begin(),
			nowState->nextStepVec.end(),
			cmp
		);


		//if(nowState->nextStepVec.size()> LIMIT_WIDTH)
		//	nowState->nextStepVec.resize(LIMIT_WIDTH);
	}

	if (fast_search >0)
	{
		nowState->nextStepVec.clear();
		//fast_search = 1: black attack, use nowState->nextStepMap[0]
		for (auto& v : nowState->nextStepMap[((fast_search==1)?(0):(1))])
		{
			nowState->nextStepVec.emplace_back(v.first, (v.second));
		}

		//for (auto& v : nowState->nextStepMap[is_min ^ 1])
		//{
		//	nowState->nextStepVec.emplace_back(v.first, (v.second));
		//}

		//fast_search = 1: black attack, prior for high score
		auto cmp = [&](const std::pair<std::pair<int, int>, ll> a,
			const std::pair<std::pair<int, int>, ll> b)
		{
			return ((fast_search==2) ? (a.second<b.second) : (a.second>b.second));
			
			//return (a.second) > (b.second);
		};

		std::sort(
			nowState->nextStepVec.begin(),
			nowState->nextStepVec.end(),
			cmp
		);
	}
	
	nowState->actualScore = ACT_INIT * ((is_min) ? 1 : -1); //init actual score
	//tie judge...



	//for (auto &v : nowState->nextStepVec)

	int vec_index_a = 0;
	int vec_index_b = nowState->nextStepVec.size()-1;

	for(int i=1;i<= nowState->nextStepVec.size();i++)
	{
		std::pair<std::pair<int, int>, ll> v;
		if (vec_index_a > vec_index_b) break;
		if (i%2==is_min^1)
		{
			v = nowState->nextStepVec[vec_index_a++];
		}
		else
		{
			v = nowState->nextStepVec[vec_index_b--];
		}

		++limit_width_cnt ;
		if (limit_width_cnt > limit_width) break;

		auto& pos = v.first;
		auto& heuristic_score = v.second;

		if (alpha >= beta)
		{
			++stc.alpha_beta_cnt;
			break; //alpha-beta pruning
		}

		tmpGameBoard->putPiece(pos.first, pos.second, tmpGameBoard->getWho());
		
		auto nState = Search_B(
			depth + 1,
			search_id,
			stateMap,
			tmpGameBoard,
			nowState,
			alpha,
			beta,
			limitDepth,
			pos.first,
			pos.second,
			is_min ^ 1,
			fast_search,
			stc
		);

		if (is_min)
		{
			beta = std::min(beta, nState->actualScore);

			//update min node val
			if (nState->actualScore < nowState->actualScore)
			{
				nowState->actualScore = nState->actualScore;
				nowState->ans = pos;
				nowState->ansHashVal = tmpGameBoard->hashVal;
			}
		}
		else
		{
			alpha = std::max(alpha, nState->actualScore);

			//update max node val
			if (nState->actualScore > nowState->actualScore)
			{
				nowState->actualScore = nState->actualScore;
				nowState->ans = pos;
				nowState->ansHashVal = tmpGameBoard->hashVal;
			}
		}

		tmpGameBoard->popPiece();

	}

	if (fast_search>0) nowState->nextStepVec.clear();

	return
		nowState;

}

#include <iostream>

int LIMIT_DEPTH = 10;
int LIMIT_WIDTH = 11;

std::pair<st_pt, std::pair<int, int>> Search_A(int search_id, StateMap &stateMap, gb_pt tmpGameBoard, st_pt rootState, int is_white)
{
	//Begin
	if (tmpGameBoard->piecePos.size() == 0)
	{
		int newx = 7;
		int newy = 7;

		tmpGameBoard->putPiece(7, 7, is_white);
		//update by hand
		auto nowState = std::make_shared<State>(*rootState);

		//update hashval
		nowState->hashVal = tmpGameBoard->hashVal;

		//update State score
		nowState->updateEstimatedScore(tmpGameBoard, newx, newy);

		//update Next Step Map
		nowState->updateNextStepMap(tmpGameBoard, newx, newy);

		//add this state to stateMap
		stateMap[tmpGameBoard->hashVal] = { nowState, search_id };

		tmpGameBoard->popPiece();
		return { nowState, {newx,newy} };
	}

	auto &last_move = tmpGameBoard->piecePos.back();
	//int cnt = 0;
	Static stc;

	int depth = LIMIT_DEPTH;
	int use_fast_cnt = -1;

	if (tmpGameBoard->piecePos.size() < 3)
	{
		depth = 7;
		LIMIT_WIDTH = 8;
	}
	else if (tmpGameBoard->piecePos.size() < 5)
	{
		depth = 9;
		LIMIT_WIDTH = 8;
	}
	else if (tmpGameBoard->piecePos.size() < 9)
	{
		depth = 11;
		LIMIT_WIDTH = 10;
	}
	else
	{
		depth = 13;
		LIMIT_WIDTH = 9;
	}

	if (tmpGameBoard->piecePos.size() < 5)
	{
		use_fast_cnt = -1;
	}
	

	auto nState = Search_B(
		1,
		search_id,
		stateMap,
		tmpGameBoard,
		rootState,
		ALPHA_INIT,
		BETA_INIT,
		depth,
		last_move.first,
		last_move.second,
		is_white,
		use_fast_cnt,
		stc
	);

	stc.printCnt();

	std::cerr << "ANS:" << (nState->ans).first << " " << (nState->ans).second << std::endl;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			for (int k = 0; k < 6; k++)
			{
				std::cerr << nState->dragonState.dragon[i][j][k] << " ";
				//assert(dragonState.dragon[i][j][k] >= 0);
			}
			std::cerr << std::endl;
		}
		std::cerr << "---" << std::endl;
	}

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			for (int k = 0; k < 6; k++)
			{
				std::cerr << stateMap[nState->ansHashVal].first->dragonState.dragon[i][j][k] << " ";
				//assert(dragonState.dragon[i][j][k] >= 0);
			}
			std::cerr << std::endl;
		}
		std::cerr << "---" << std::endl;
	}

	return {
		stateMap[nState->ansHashVal].first,
		nState->ans
	};

	
}

ll calculateEstimatedScoreByDragonStatic(DragonStatic& ds)
{
	ll ans = 0;


	for (int k = 1; k < 6; k++)
	{
		ans += HUO_DRAGON_SCORE[k] * ds.dragon[0][0][k];
	}

	for (int k = 1; k < 6; k++)
	{
		ans += CHONG_DRAGON_SCORE[k] * ds.dragon[0][1][k];
	}

	for (int k = 1; k < 6; k++)
	{
		ans -= HUO_DRAGON_SCORE[k] * ds.dragon[1][0][k];
	}

	for (int k = 1; k < 6; k++)
	{
		ans -= CHONG_DRAGON_SCORE[k] * ds.dragon[1][1][k];
	}

	return ans;
}

void bruteforceCheckDragon(DragonStatic& ds, gb_pt& gb)
{
	int tmp_dr[2][2][6];
	memset(tmp_dr, 0, sizeof(tmp_dr));

	for (int i=0;i<N;i++)
	{
		for (int j = 0; j < N; j++)
		{
			judgeDragon_A_0(gb, i, j, tmp_dr);
			judgeDragon_A_1(gb, i, j, tmp_dr);
			judgeDragon_A_2(gb, i, j, tmp_dr);
			judgeDragon_A_3(gb, i, j, tmp_dr);
		}
	}
	
	
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			for (int k = 0; k < 6; k++)
			{
				if (tmp_dr[i][j][k]!=ds.dragon[i][j][k])
				{
					throw "bruteforceCheckDragon DIFFERENT!";
				}
			}
		}
	}
}

//do not copy nextStepVec
State::State(const State& b):
	estimatedScore(b.estimatedScore),
	actualScore(b.actualScore),
	ans(b.ans),
	ansHashVal(ansHashVal),
	nextStepMap(b.nextStepMap),
	pieceCnt(b.pieceCnt),
	dragonState(b.dragonState),
	hashVal(b.hashVal)
{
}

// has optimization space...
void State::updateEstimatedScore(gb_pt tmpGameBoard, int x, int y)
{
	dragonState.whoNext = tmpGameBoard->getWho();

	//set piece_cnt
	pieceCnt = tmpGameBoard->piecePos.size();

	ll before_score, after_score;
	//undo
	auto last_move = tmpGameBoard->fakePopPiece(x,y);

	int dragonState_before[2][2][6]; memset(dragonState_before, 0, sizeof(dragonState_before));
	before_score = judgeDragon_A(tmpGameBoard, x, y, 15, dragonState_before);

	//reset
	tmpGameBoard->fakePutPiece(x, y, last_move);

	int dragonState_after[2][2][6]; memset(dragonState_after, 0, sizeof(dragonState_after));
	after_score = judgeDragon_A(tmpGameBoard, x, y, 15, dragonState_after);


	//preprocess dragonState_after
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			for (int k = 0; k < 6; k++)
			{
				dragonState_after[i][j][k] -= dragonState_before[i][j][k];
				dragonState.dragon[i][j][k] += dragonState_after[i][j][k];
				assert(dragonState.dragon[i][j][k] >= 0);
			}
		}
	}

	//black
	     if (dragonState.dragon[0][0][5]) estimatedScore = WIN_THRESHOLD;
	else if (dragonState.dragon[1][0][5]) estimatedScore = -WIN_THRESHOLD;

	else if (dragonState.dragon[0][0][4] && dragonState.whoNext == 0) estimatedScore = PREWIN_THRESHOLD;
	else if (dragonState.dragon[0][1][4] && dragonState.whoNext == 0) estimatedScore = PREWIN_THRESHOLD;
	else if (dragonState.dragon[0][0][4] && dragonState.dragon[1][0][4] == 0 && dragonState.dragon[1][1][4] == 0 && dragonState.whoNext == 1) estimatedScore = PREWIN_THRESHOLD;
	else if (dragonState.dragon[0][0][3] && dragonState.dragon[1][0][4] == 0 && dragonState.dragon[1][1][4] == 0 && dragonState.whoNext == 0) estimatedScore = PREWIN_THRESHOLD;
	else if (dragonState.dragon[0][1][4] && dragonState.dragon[0][0][3] && dragonState.dragon[1][0][4] == 0 && dragonState.dragon[1][1][4] == 0 && dragonState.whoNext == 1) estimatedScore = PREWIN_THRESHOLD;
	else if (dragonState.dragon[0][0][3] >= 2 && dragonState.dragon[1][0][4] == 0 && dragonState.dragon[1][1][4] == 0 && dragonState.dragon[1][0][3] == 0 && dragonState.dragon[1][1][3] == 0 && dragonState.whoNext == 1) estimatedScore = PREWIN_THRESHOLD;

	//white
	else if (dragonState.dragon[1][0][4] && dragonState.whoNext == 1) estimatedScore = -PREWIN_THRESHOLD;
	else if (dragonState.dragon[1][1][4] && dragonState.whoNext == 1) estimatedScore = -PREWIN_THRESHOLD;
	else if (dragonState.dragon[1][0][4] && dragonState.dragon[0][0][4] == 0 && dragonState.dragon[0][1][4] == 0 && dragonState.whoNext == 0) estimatedScore = -PREWIN_THRESHOLD;
	else if (dragonState.dragon[1][0][3] && dragonState.dragon[0][0][4] == 0 && dragonState.dragon[0][1][4] == 0 && dragonState.whoNext == 1) estimatedScore = -PREWIN_THRESHOLD;
	else if (dragonState.dragon[1][1][4] && dragonState.dragon[1][0][3] && dragonState.dragon[0][0][4] == 0 && dragonState.dragon[0][1][4] == 0 && dragonState.whoNext == 0) estimatedScore = -PREWIN_THRESHOLD;
	else if (dragonState.dragon[1][0][3] >= 2 && dragonState.dragon[0][0][4] == 0 && dragonState.dragon[0][1][4] == 0 && dragonState.dragon[0][0][3] == 0 && dragonState.dragon[0][1][3] == 0 && dragonState.whoNext == 0) estimatedScore = -PREWIN_THRESHOLD;

	//else
	//	 estimatedScore += after_score - before_score;
	else 
	     estimatedScore = calculateEstimatedScoreByDragonStatic(dragonState);
	//assert(!(std::abs(estimatedScore) < PREWIN_THRESHOLD&& estimatedScore != tmp));

	//regular
	//if (estimatedScore >= WIN_THRESHOLD)
	//	estimatedScore = WIN_THRESHOLD;

	//if (estimatedScore <= -WIN_THRESHOLD)
	//	estimatedScore = -WIN_THRESHOLD;
}


const int NEW_NEXT_DIS = 2;
const int UPDATE_DIS = 4;
void State::updateNextStepMap(gb_pt tmpGameBoard, int x, int y)
{
	nextStepMap[0].erase({ x,y });
	nextStepMap[1].erase({ x,y });

	auto __update_0 = [&](int xop, int yop, int mask) {
		ll before_score, after_score;
		for (int i = -UPDATE_DIS; i <= UPDATE_DIS; i++)
		{
			int xx = x + i * xop;
			int yy = y + i * yop;
			if (tmpGameBoard->getWhoPiece(xx,yy) == 2) //none
			{
				bool is_find = (nextStepMap[0].find({ xx,yy }) != nextStepMap[0].end());
				if ( (abs(i)<=NEW_NEXT_DIS ) || (is_find))
				{
				//black
					tmpGameBoard->fakePutPiece(xx, yy, 1);
					after_score = judgeDragon_A(tmpGameBoard, xx, yy, mask);
					tmpGameBoard->fakePopPiece(xx, yy);

					nextStepMap[0][{xx, yy}] = after_score ;
				

				//white

					tmpGameBoard->fakePutPiece(xx, yy, 2);
					after_score = judgeDragon_A(tmpGameBoard, xx, yy, mask);
					tmpGameBoard->fakePopPiece(xx, yy);

					nextStepMap[1][{xx, yy}] = after_score;
				}

			}

		}
	};

	__update_0(0, 1, 15);
	__update_0(1, 1, 15);
	__update_0(1, 0, 15);
	__update_0(-1, 1, 15);
	//__update_1();
	//__update_2();
	//__update_3();
}

#include "AIController.h"
#include <utility>
#include <cmath>
#include <iostream>
#include <cstring>
#include <cstdio>
#include <algorithm>
#include <queue>
#pragma GCC optimize(3,"Ofast","inline")
#include <stdlib.h>
extern int ai_side; // 0: black, 1: white
// int ai_side=1;
std::string ai_name = "your_ai_name_here";
const int INF = 1e9;
const int start_depth = 6;
const int vax_depth=6;
const int count_limit=10;
int step;
int vax_time;
int turn = 0;
int board[15][15];
int evaluating_point[6][3] = {{0, 0, 0}, {0, 0, 1}, {0, 1, 3}, {0, 3, 1000}, {0, 1000, 100000}, {1000000, 1000000, 1000000}};
int search_num;
struct Node
{
    std::pair<int, int> cur_location;
    int value;
};
// init function is called once at the beginning
void init()
{
    /* TODO: Replace this by your code */
    memset(board, -1, sizeof(board));
}

// loc is the action of your opponent
// Initially, loc being (-1,-1) means it's your first move
// If this is the third step(with 2 black ), where you can use the swap rule, your output could be either (-1, -1) to indicate that you choose a swap, or a coordinate (x,y) as normal.
bool is_legal(int pos_x, int pos_y)
{
    if (pos_x < 0 || pos_y < 0 || pos_x >= 15 || pos_y >= 15)
        return 0;
    return 1;
}
int jud_free(int pos_x, int pos_y)
{
    if (is_legal(pos_x, pos_y) == 0 || board[pos_x][pos_y] != -1)
        return 0;
    return 1;
}
int evaluating_fuc(int cur_map[15][15], int cur_side)
{
    int num[6][3];
    memset(num,0,sizeof(num));
    int legal_num;
    int lenth;
    int is_jump=0;
    for (int i = 0; i < 15; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            is_jump=lenth=0;
            for (int k = 0;; k++)
            { //横向搜
                if(!is_legal(i,j+k+is_jump))break;
                if(cur_map[i][j+k]==-1&&is_legal(i,j+k+1)&&cur_map[i][j+k+1]==cur_side&&!is_jump){
                    is_jump=1;
                }
                if(cur_map[i][j+k+is_jump]!=cur_side)break;
                lenth = k+1;
                if(lenth>5){
                    lenth=5;
                    break;
                }
            } 
            if(lenth>=5&&is_jump)lenth=4,legal_num=1+jud_free(i, j - 1);
            else legal_num = jud_free(i, j + lenth+is_jump) + jud_free(i, j - 1);
            num[lenth][legal_num] += 1; // count num

            is_jump=lenth=0;
            for (int k = 0; ; k++)
            { //纵向搜
                if(!is_legal(i+k+is_jump,j))break;
                if(cur_map[i+k][j]==-1&&is_legal(i+k+1,j)&&cur_map[i+k+1][j]==cur_side&&!is_jump)
                    is_jump=1;
                if(cur_map[i+k+is_jump][j]!=cur_side)break;
                lenth=k+1;
                if(lenth>5){
                    lenth=5;
                    break;
                }
            }
            if(lenth>=5&&is_jump)lenth=4,legal_num=1+jud_free(i-1, j);
            else legal_num = jud_free(i + lenth+is_jump, j) + jud_free(i - 1, j);
            num[lenth][legal_num] += 1;
            is_jump=lenth=0;
            for (int k = 0;; k++)
            { //右斜搜
                if(!is_legal(i+k+is_jump,j+k+is_jump))break;
                if(cur_map[i+k][j+k]==-1&&is_legal(i+k+1,j+k+1)&&cur_map[i+k+1][j+k+1]==cur_side&&!is_jump)
                    is_jump=1;
                if(cur_map[i+k+is_jump][j+k+is_jump]!=cur_side)break;
                lenth = k+1;
                if(lenth>5){
                    lenth=5;
                    break;
                }
            }
            if(lenth>=5&&is_jump)lenth=4,legal_num=1+jud_free(i-1, j-1);
            else legal_num = jud_free(i + lenth+is_jump, j + lenth+is_jump) + jud_free(i - 1, j - 1);
            num[lenth][legal_num] += 1;
            is_jump=lenth=0;
            for (int k = 0;; k++)
            { //左斜搜
                if(!is_legal(i+k+is_jump,j-k-is_jump))break;
                if(cur_map[i+k][j-k]==-1&&is_legal(i+k+1,j-k-1)&&cur_map[i+k+1][j-k-1]==cur_side&&!is_jump)
                    is_jump=1;
                if(cur_map[i+k+is_jump][j-k-is_jump]!=cur_side)break;
                lenth =k+1;
                if(lenth>5){
                    lenth=5;
                    break;
                }
            }
            if(lenth>=5&&is_jump)lenth=4,legal_num=1+jud_free(i - 1, j + 1);
            else legal_num = jud_free(i + lenth+is_jump, j - lenth-is_jump) + jud_free(i - 1, j + 1);
            num[lenth][legal_num] += 1;
        }
    }
    int ans = 0;
    for(int i=1;i<=5;i++){
        for(int j=0;j<=2;j++){
            ans+=num[i][j]*evaluating_point[i][j];
        }
    }
    return ans;
}
std::pair<int, int> getRandom()
{
    while (true)
    {
        int x = rand() % 15;
        int y = rand() % 15;
        if (board[x][y] == -1)
        {
            board[x][y] = ai_side;
            return std::make_pair(x, y);
        }
    }
}
bool jud_role(int pos_x, int pos_y, int role)
{
    if (is_legal(pos_x, pos_y) && board[pos_x][pos_y] == role)
        return 1;
    return 0;
}
void Print()
{
    for (int i = 0; i < 15; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            if (board[i][j] == -1)
                std::cerr << '-';
            else
                std::cerr << board[i][j];
            // std::cerr<<((char)board[i][j]==-1?'-':(int)board[i][j])<<" ";
            std::cerr << " ";
        }
        std::cerr << '\n';
    }
    std::cerr << '\n';
}
int evaluat_position(int pos_x, int pos_y, int role,bool &is_block_four)
{ //对点进行评估
    int left_p[2] = {0}, right_p[2] = {0}, ans = 0;
    int legal_num;
    int lenth;
    is_block_four=0;
    int tmp_num;
    int is_jump_l,is_jump_r;
    is_jump_l=is_jump_r=0;
    legal_num=0;
    lenth=0;
    int l_lenth,r_lenth,_lenth;
    right_p[0]=right_p[1]=left_p[0]=left_p[1]=0;
    for (int i = 1; i <5; i++){
        if(!is_legal(pos_x+i+is_jump_r,pos_y))break;
        if(jud_free(pos_x+i,pos_y)&&jud_role(pos_x+i+1,pos_y,role)&&is_jump_r==0){
            is_jump_r=1;
        }
        if(!jud_role(pos_x+i+is_jump_r,pos_y,role))break;
        right_p[is_jump_r]=i;
    }
    for(int i=1;i<5;i++){
        if(!is_legal(pos_x-i-is_jump_l,pos_y))break;
        if(jud_free(pos_x-i,pos_y)&&jud_role(pos_x-i-1,pos_y,role)&&is_jump_l==0){
            is_jump_l=1;
        }
        if(!jud_role(pos_x-i-is_jump_l,pos_y,role))break;
        left_p[is_jump_l]=i;
    }
    l_lenth=left_p[1]+right_p[0]+1,r_lenth=right_p[1]+left_p[0]+1,_lenth=left_p[0]+right_p[0]+1;
    lenth=_lenth;
    legal_num=jud_free(pos_x-left_p[0]-1,pos_y)+jud_free(pos_x+right_p[0]+1,pos_y);
    if(l_lenth>=lenth&&l_lenth<4){
        tmp_num=jud_free(pos_x-left_p[1]-2,pos_y)+jud_free(pos_x+right_p[0]+1,pos_y);
        if(l_lenth>lenth){
            lenth=l_lenth;legal_num=tmp_num;
        }
        else if(tmp_num>legal_num)legal_num=tmp_num;
    }
    if(r_lenth>=lenth&&r_lenth<4){
        tmp_num=jud_free(pos_x-left_p[0]-1,pos_y)+jud_free(pos_x+right_p[1]+2,pos_y);
        if(r_lenth>lenth){
            lenth=r_lenth,legal_num=tmp_num;
        }
        else if(tmp_num>legal_num)legal_num=tmp_num;
    }
    lenth = std::min(lenth, 5);
    ans += evaluating_point[lenth][legal_num];
    if(lenth==4&&legal_num==1)is_block_four=1;
    
    is_jump_l=is_jump_r=0;
    legal_num=0;
    lenth=0;
    right_p[0]=right_p[1]=left_p[0]=left_p[1]=0;//纵向
    for (int i = 1; i <5; i++){
        if(!is_legal(pos_x,pos_y+i+is_jump_r))break;
        if(jud_free(pos_x,pos_y+i)&&jud_role(pos_x,pos_y+i+1,role)&&is_jump_r==0){
            is_jump_r=1;
        }
        if(!jud_role(pos_x,pos_y+i+is_jump_r,role))break;
        right_p[is_jump_r]=i;
    }
    for(int i=1;i<5;i++){
        if(!is_legal(pos_x,pos_y-i-is_jump_l))break;
        if(jud_free(pos_x,pos_y-i)&&jud_role(pos_x,pos_y-i-1,role)&&is_jump_l==0){
            is_jump_l=1;
        }
        if(!jud_role(pos_x,pos_y-i-is_jump_l,role))break;
        left_p[is_jump_l]=i;
    }
    l_lenth=left_p[1]+right_p[0]+1,r_lenth=right_p[1]+left_p[0]+1,_lenth=left_p[0]+right_p[0]+1;
    lenth=_lenth;
    legal_num=jud_free(pos_x,pos_y-left_p[0]-1)+jud_free(pos_x,pos_y+right_p[0]+1);
    if(l_lenth>=lenth&&l_lenth<4){
        tmp_num=jud_free(pos_x,pos_y-left_p[1]-2)+jud_free(pos_x,pos_y+right_p[0]+1);
        if(l_lenth>lenth){
            lenth=l_lenth;
            legal_num=tmp_num;
        }
        else if(tmp_num>legal_num)legal_num=tmp_num;
    }
    if(r_lenth>=lenth&&r_lenth<4){
        tmp_num=jud_free(pos_x,pos_y+right_p[1]+2)+jud_free(pos_x,pos_y-left_p[0]-1);
        if(r_lenth>lenth){
            lenth=r_lenth;
            legal_num=tmp_num;
        }
        else if(tmp_num>legal_num)legal_num=tmp_num;
    }
    lenth = std::min(lenth, 5);
    ans += evaluating_point[lenth][legal_num];
    if(lenth==4&&legal_num==1)is_block_four=1;
    
    is_jump_l=is_jump_r=0;
    legal_num=0;
    lenth=0;
    right_p[0]=right_p[1]=left_p[0]=left_p[1]=0; //右斜
    for (int i = 1; i <5; i++){
        if(!is_legal(pos_x+i+is_jump_r,pos_y+i+is_jump_r))break;
        if(jud_free(pos_x+i,pos_y+i)&&jud_role(pos_x+i+1,pos_y+i+1,role)&&is_jump_r==0){
            is_jump_r=1;
        }
        if(!jud_role(pos_x+i+is_jump_r,pos_y+i+is_jump_r,role))break;
        right_p[is_jump_r]=i;
    }
    for(int i=1;i<5;i++){
        if(!is_legal(pos_x-i-is_jump_l,pos_y-i-is_jump_l))break;
        if(jud_free(pos_x-i,pos_y-i)&&jud_role(pos_x-i-1,pos_y-i-1,role)&&is_jump_l==0){
            is_jump_l=1;
        }
        if(!jud_role(pos_x-i-is_jump_l,pos_y-i-is_jump_l,role))break;
        left_p[is_jump_l]=i;
    }
    l_lenth=left_p[1]+right_p[0]+1,r_lenth=right_p[1]+left_p[0]+1,_lenth=left_p[0]+right_p[0]+1;
    lenth=_lenth;
    legal_num=jud_free(pos_x+right_p[0]+1,pos_y+right_p[0]+1)+jud_free(pos_x-left_p[0]-1,pos_y-left_p[0]-1);
    if(l_lenth>=lenth&&l_lenth<4){
        tmp_num=jud_free(pos_x-left_p[1]-2,pos_y-left_p[1]-2)+jud_free(pos_x+right_p[0]+1,pos_y+right_p[0]+1);
        if(l_lenth>lenth){
            lenth=l_lenth;legal_num=tmp_num;
        }
        else if(tmp_num>legal_num)legal_num=tmp_num;
    }
    if(r_lenth>=lenth&&r_lenth<4){
        tmp_num=jud_free(pos_x-left_p[0]-1,pos_y-left_p[0]-1)+jud_free(pos_x+right_p[1]+2,pos_y+right_p[1]+2);
        if(r_lenth>lenth){
            lenth=r_lenth;legal_num=tmp_num;
        }
        else if(tmp_num>legal_num)legal_num=tmp_num;
    }
    lenth = std::min(lenth, 5);
    ans += evaluating_point[lenth][legal_num];
    if(lenth==4&&legal_num==1)is_block_four=1;

    is_jump_l=is_jump_r=0;
    legal_num=0;
    lenth=0;
    right_p[0]=right_p[1]=left_p[0]=left_p[1]=0;//左斜
    for (int i = 1; i <5; i++){
        if(!is_legal(pos_x-i-is_jump_r,pos_y+i+is_jump_r))break;
        if(jud_free(pos_x-i,pos_y+i)&&jud_role(pos_x-i-1,pos_y+i+1,role)&&is_jump_r==0){
            is_jump_r=1;
        }
        if(!jud_role(pos_x-i-is_jump_r,pos_y+i+is_jump_r,role))break;
        right_p[is_jump_r]=i;
    }
    for(int i=1;i<5;i++){
        if(!is_legal(pos_x+i+is_jump_l,pos_y-i-is_jump_l))break;
        if(jud_free(pos_x+i,pos_y-i)&&jud_role(pos_x+i+1,pos_y-i-1,role)&&is_jump_l==0){
            is_jump_l=1;
        }
        if(!jud_role(pos_x+i+is_jump_l,pos_y-i-is_jump_l,role))break;
        left_p[is_jump_l]=i;
    }
    l_lenth=left_p[1]+right_p[0]+1,r_lenth=right_p[1]+left_p[0]+1,_lenth=left_p[0]+right_p[0]+1;
    lenth=_lenth;
    legal_num=jud_free(pos_x-right_p[0]-1,pos_y+right_p[0]+1)+jud_free(pos_x+left_p[0]+1,pos_y-left_p[0]-1);
    if(l_lenth>=lenth&&l_lenth<4){
        tmp_num=jud_free(pos_x-right_p[0]-1,pos_y+right_p[0]+1)+jud_free(pos_x+left_p[1]+2,pos_y-left_p[1]-2);
        if(l_lenth>lenth){
            lenth=l_lenth;legal_num=tmp_num;
            if(lenth>=5)lenth=4;
        }
        else if(tmp_num>legal_num)legal_num=tmp_num;
    }
    if(r_lenth>=lenth&&r_lenth<4){
        tmp_num=jud_free(pos_x-right_p[1]-2,pos_y+right_p[1]+2)+jud_free(pos_x+left_p[0]+1,pos_y-left_p[0]-1);
        if(r_lenth>lenth){
            lenth=r_lenth;legal_num=tmp_num;
            if(lenth>=5)lenth=4;
        }
        else if(tmp_num>legal_num)legal_num=tmp_num;
    }
    lenth = std::min(lenth, 5);
    ans += evaluating_point[lenth][legal_num];
    if(lenth==4&&legal_num==1)is_block_four=1;

    // if(ans>evaluating_point[3][2]){
    //     std::cerr<<pos_x<<" "<<pos_y<<" "<<role<<'\n';
    //     Print();
    // }
    return ans;
}
bool exist_neighbor(int pos_x, int pos_y) //判断是否有neighbor
{
    for (int i = -1; i <= 1; i++){
        for (int j = -1; j <= 1; j++){
            if (pos_x + i < 15 && pos_x + i >= 0 && pos_y + j < 15 && pos_y + j >= 0 && board[pos_x + i][pos_y + j] != -1)
                return 1;
        }
    }
    return 0;
}
void merge(std::vector<Node> &a, std::vector<Node> &b,
           std::vector<Node> &point_list)
{
    std::pair<int,int> position;
    int size_a = a.size(), size_b = b.size();
    int size_p=point_list.size();
    int vis[15][15];
    memset(vis, 0, sizeof(vis));
    for(int i=0;i<size_p;i++){
        position=point_list[i].cur_location;
        vis[position.first][position.second]=1;
    }
    for (int i = 0; i < size_a; i++)
    {
        position = a[i].cur_location;
        if (!vis[position.first][position.second])
            point_list.push_back(a[i]);
        vis[position.first][position.second] = 1;
    }
    for (int i = 0; i < size_b; i++)
    {
        position = b[i].cur_location;
        if (!vis[position.first][position.second])
            point_list.push_back(b[i]);
        vis[position.first][position.second] = 1;
    }
}
bool Node_cmp(Node &aa,Node &bb){return aa.value>bb.value;}
void generator(int role, std::vector<Node> &point_list)
{
    std::vector<Node> five, role_four, oppo_four, role_block_four,
        oppo_block_four, role_double_three, oppo_double_three, role_three, oppo_three,
        tmp;
    std::vector<Node>neighbor;
    Node store;
    int role_score, oppo_score;
    bool role_is_block_four,oppo_is_block_four;
    Node position;
    for (int i = 0; i < 15; i++){
        for (int j = 0; j < 15; j++){
            if (board[i][j] == -1 && exist_neighbor(i, j)){
                position.cur_location.first = i, position.cur_location.second = j;
                role_score = evaluat_position(i, j, role,role_is_block_four);
                oppo_score = evaluat_position(i, j, role ^ 1,oppo_is_block_four);
                position.value=role_score;
                if (role_score >= evaluating_point[5][0] || oppo_score > evaluating_point[5][0])
                {
                    five.push_back(position);
                    continue;
                }
                if (role_score >= evaluating_point[4][2])
                    role_four.push_back(position); // counting role
                else if (role_score >= evaluating_point[4][1]&&role_is_block_four)
                    role_block_four.push_back(position);
                else if (role_score >= 2 * evaluating_point[3][2])
                    role_double_three.push_back(position);
                else if (role_score >= evaluating_point[3][2])
                    role_three.push_back(position);
                else{
                    neighbor.push_back(position);
                } 
                position.value=oppo_score;
                if (oppo_score >= evaluating_point[4][2])
                    oppo_four.push_back(position);
                else if (oppo_score >= evaluating_point[4][1]&&oppo_is_block_four)
                    oppo_block_four.push_back(position);
                else if (oppo_score >= 2 * evaluating_point[3][2])
                    oppo_double_three.push_back(position);
                else if (oppo_score >= evaluating_point[3][2])
                    oppo_three.push_back(position);
                else{
                    if(neighbor.size()&&neighbor.back().cur_location==position.cur_location){
                        if(neighbor.back().value<oppo_score)neighbor.back().value=oppo_score;
                    }
                    else {
                        neighbor.push_back(position);
                    }
                }
            }
        }
    }
    if(five.size())sort(five.begin(),five.end(),Node_cmp);
    if(role_four.size())sort(role_four.begin(),role_four.end(),Node_cmp);
    if(oppo_four.size())sort(oppo_four.begin(),oppo_four.end(),Node_cmp);
    if(role_double_three.size())sort(role_double_three.begin(),role_double_three.end(),Node_cmp);
    if(oppo_double_three.size())sort(oppo_double_three.begin(),oppo_double_three.end(),Node_cmp);
    if(role_three.size())sort(role_three.begin(),role_three.end(),Node_cmp);
    if(oppo_three.size())sort(oppo_three.begin(),oppo_three.end(),Node_cmp);//TO DO： evaluat_point函数有问题
    if(role_block_four.size())sort(role_block_four.begin(),role_block_four.end(),Node_cmp);
    if(oppo_block_four.size())sort(oppo_block_four.begin(),oppo_block_four.end(),Node_cmp);

    if (five.size())point_list = five;
    else if (role_four.size())point_list = role_four;
    else if (oppo_four.size())merge(oppo_four, role_block_four, point_list);
    else if (role_double_three.size())merge(role_double_three, role_block_four, point_list);
    else if (oppo_double_three.size())merge(oppo_double_three,role_block_four, point_list);
    else if (role_three.size())merge(role_three, role_block_four, point_list);
    else if (oppo_three.size()){
        merge(role_block_four, oppo_block_four, tmp);
        merge(oppo_three, tmp, point_list);
    }
    else {
        merge(role_block_four,oppo_block_four,point_list);
        sort(neighbor.begin(),neighbor.end(),Node_cmp);
        int cnt=0;
        while(point_list.size()<count_limit&&cnt<neighbor.size()){
            point_list.push_back(neighbor[cnt]);
            cnt++;
        }
    }
   // if(point_list[0].value>evaluating_point[3][2])std::cerr<<"SZ is :"<<point_list.size()<<'\n';
}
int get_score(int role)
{
    int tmp = 0;
    tmp = evaluating_fuc(board, role);
    return tmp;
}
int evaluating(int cur_ai)
{
    int ai_score = 0, human_score = 0;
    ai_score = get_score(cur_ai);
    human_score = get_score(1^cur_ai);
    return ai_score - human_score;
}
void generator_vax(int role,int cur_ai,std::vector<Node> &point_list){
    std::vector<Node> five, role_four, oppo_four, role_block_four,
    oppo_block_four, role_three,oppo_three;
    int role_score, oppo_score;
    Node position;
    bool role_is_block_four,oppo_is_block_four;
    for (int i = 0; i < 15; i++){
        for (int j = 0; j < 15; j++){
            if (board[i][j] == -1 && exist_neighbor(i, j)){
                position.cur_location.first = i, position.cur_location.second = j;
                role_score = evaluat_position(i, j, role,role_is_block_four);
                oppo_score = evaluat_position(i, j, role ^ 1,oppo_is_block_four);
                if (role_score >= evaluating_point[5][0] || oppo_score > evaluating_point[5][0])
                {
                    five.push_back(position);
                    continue;
                }
                if (role_score >= evaluating_point[4][2])
                    role_four.push_back(position); // counting role
                else if (role_score >= evaluating_point[4][1]&&role_is_block_four)
                    role_block_four.push_back(position);
                else if (role_score >= evaluating_point[3][2])
                    role_three.push_back(position);
                   
                if (oppo_score >= evaluating_point[4][2])
                    oppo_four.push_back(position);
                else if (oppo_score >= evaluating_point[4][1]&&oppo_is_block_four)
                    oppo_block_four.push_back(position);
                else if(oppo_score>=evaluating_point[3][2])
                    oppo_three.push_back(position);
            }
        }
    }
    if(five.size())point_list=five;
    else if(role_four.size())point_list=role_four;
    else if(oppo_four.size())merge(oppo_four,role_block_four,point_list);
    else {
        if(role==cur_ai)merge(role_three,role_block_four,point_list);
        else{
            merge(role_three,oppo_three,point_list);
            merge(oppo_block_four,role_block_four,point_list);
        }
    }
}
Node evaluate_vax(int cur_ai,int cur_side,int alpha,int beta,int cur_depth){
    std::vector<Node> point_list;
    search_num++;
    Node cur_pos;
    Node ans;ans.value=-INF;
    int oppo_score,role_score;
    oppo_score=get_score(1^cur_side);
    role_score=get_score(cur_side);
    if(role_score>=evaluating_point[5][0]){
        ans.value=evaluating_point[5][0];return  ans;
    }
    if(oppo_score>=evaluating_point[5][0]){
        ans.value=-1*evaluating_point[5][0];return ans;
    }
    if(cur_depth==0){
        ans.value=-1*evaluating_point[5][0];
        return ans;
    }
    generator_vax(cur_side, cur_ai,point_list);
    int sz = point_list.size();
    if(sz==0){
        if(cur_ai==cur_side)ans.value= -1*evaluating_point[5][0];
        else ans.value=evaluating_point[5][0];
        return ans;
    }
    int i,j; 
    for (int k = 0; k < sz; k++){
        i=point_list[k].cur_location.first,j=point_list[k].cur_location.second;
        board[i][j] = cur_side;
        cur_pos = evaluate_vax(cur_ai, 1^cur_side, -1 * beta, -1 * alpha, cur_depth - 1);
        board[i][j] = -1;
        cur_pos.value *= -1;
        alpha = std::max(alpha, cur_pos.value);
        if (cur_pos.value > ans.value)
        {
            ans.cur_location.first = i, ans.cur_location.second = j;
            ans.value = cur_pos.value;
        }
        if(cur_ai==cur_side&&ans.value>=evaluating_point[5][0])return ans;
        if(cur_ai!=cur_side&&ans.value>-1*evaluating_point[5][0])return ans;
        if (ans.value > beta){
             return ans;
        }
    }
    return ans;
}

Node alphabeta(int steps, int cur_ai, int cur_side, int alpha, int beta, int cur_depth)
{ // steps为已走步数
    Node cur_pos, ans;
    ans.value = -INF;
    int oppo_score,role_score;
    oppo_score=get_score(1^cur_side);
    role_score=get_score(cur_side);
    if (role_score >=evaluating_point[5][0]){
        ans.value = evaluating_point[5][0];
        return ans;
    }
    if (oppo_score >=evaluating_point[5][0]){
        ans.value = -1 * evaluating_point[5][0];
        return ans;
    }
    if (cur_depth == 0){
        ans.value = evaluating(cur_ai);
       Node is_vax=evaluate_vax(cur_ai,cur_side,alpha,beta,std::max(vax_depth-2*vax_time,2));
       if(is_vax.value>=evaluating_point[5][0]){
        ans.value=evaluating_point[5][0];
        }
        return ans;
    }
    ans.value = -INF;
    if (steps == 3){ // swap 2
        cur_pos = alphabeta(steps + 1, 1^cur_ai, cur_side, beta * -1, alpha * -1, cur_depth - 1);
        cur_pos.value *= -1;
        alpha = std::max(alpha, cur_pos.value);
        if (cur_pos.value > ans.value)
        {
            ans.cur_location.first = -1, ans.cur_location.second = -1;
            ans.value = cur_pos.value;
        }
        if (ans.value > beta){
           ans.value=INF-1;
            return ans;
        }
    }
    std::vector<Node> point_list;
    generator(cur_side, point_list);
    int sz = point_list.size();
    //if(point_list[0].value>evaluating_point[3][2])sz=std::min(sz,10);
    int i,j;
    for (int k = 0; k < sz; k++){
        i=point_list[k].cur_location.first,j=point_list[k].cur_location.second;
        board[i][j] = cur_side;
        oppo_score=get_score(1^cur_side);
        if(oppo_score>=evaluating_point[5][0])
        cur_pos=alphabeta(steps+1,cur_ai,1-cur_side,-1*beta,-1*alpha,cur_depth+1);//冲四延申
        else cur_pos = alphabeta(steps + 1, cur_ai, 1 - cur_side, -1 * beta, -1 * alpha, cur_depth - 1);
        board[i][j] = -1;
        cur_pos.value *= -1;
        alpha = std::max(alpha, cur_pos.value);
        if (cur_pos.value > ans.value)
        {
            ans.cur_location.first = i, ans.cur_location.second = j;
            ans.value = cur_pos.value;
        }
        if (ans.value > beta){
             return ans;
        }
    }
    return ans;
}
std::pair<int, int> action(std::pair<int, int> loc)
{
    /* TODO: Replace this by your code */
    /* This is now a random strategy */
   // return std::pair<int,int>(3,4);
    Node Ans;
    search_num = 0;
    turn++;
    if(turn==1&&loc.first==-1){
        board[2][7]=ai_side;
        Ans.cur_location.first=2;Ans.cur_location.second=7;
        return Ans.cur_location;
    }
    if(loc.first==-1)ai_side^=1;
    else board[loc.first][loc.second] = 1 - ai_side;
    step = turn * 2 - 2 + ai_side;
    if((turn==1&&ai_side==1)){
        if(jud_free(0,0))Ans.cur_location.first=Ans.cur_location.second=0;
        else Ans.cur_location.first=Ans.cur_location.second=14;
        board[Ans.cur_location.first][Ans.cur_location.second]=ai_side;
        return Ans.cur_location;
    }
    for(int i=2;i<=start_depth;i+=2){
        Ans = alphabeta(step, ai_side, ai_side, -INF, INF, i);
        if(i==start_depth||Ans.value>=evaluating_point[5][0]){
            if (Ans.cur_location.first != -1)
            board[Ans.cur_location.first][Ans.cur_location.second] = ai_side;
            if(Ans.cur_location.first==-1&&Ans.cur_location.second==-1)ai_side^=1;
            std::cerr<<"cur_side:"<<ai_side<<"\ncur_score:"<<Ans.value<<"\n";
            if(Ans.value>=evaluating_point[5][0]){
            //    Print();
            vax_time++;
        }
        return Ans.cur_location;
        }

    }

    
}
// int main(){
//     std::pair<int,int>human,ai_step;
//     init();
//     int cnt=1;
//     while(1){
//         //std::cin>>human.first>>human.second;
//         if(cnt==1)human.first=-1,human.second=-1;
//         if(cnt==2)human.first=0,human.second=0;
//         if(cnt==3)human.first=7,human.second=7;
//         ai_step=action(human);
        
//         cnt++;
//        if(cnt==4) break;
//     }
// }
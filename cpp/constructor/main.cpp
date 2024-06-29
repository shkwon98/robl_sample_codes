#include <iostream>
#include <memory>

typedef struct
{
    uint32_t type;
    uint32_t id;
    float c_x;
    float c_y;
    float lt_x;
    float lt_y;
    float w;
    float h;
    uint32_t p_lt_x;
    uint32_t p_lt_y;
    uint32_t p_w;
    uint32_t p_h;
    float distance;
    float bearing;
    float spd;
    float rel_spd;
    float cog;
    uint32_t obj_type;
    uint32_t col_risk;
} REC__SENSOR__FU_INFO;
#define REC__SENSOR__FU_INFO__MAX_NO    (1024)
typedef struct
{
    uint32_t fu_rec_no;
    REC__SENSOR__FU_INFO fu_rec[0];
} D__SENSOR__FU_INFO;
#define D__SENSOR__FU_INFO__MAX_SZ      (sizeof(D__SENSOR__FU_INFO) + sizeof(REC__SENSOR__FU_INFO) * REC__SENSOR__FU_INFO__MAX_NO)


int main(int argc, char** argv)
{
    auto acInfo_2 = reinterpret_cast<D__SENSOR__FU_INFO *>(new char[D__SENSOR__FU_INFO__MAX_SZ] { 0 });
    acInfo_2->fu_rec[5].type = 5;

    std::cout << acInfo_2->fu_rec[5].type << std::endl;

    if (acInfo_2 != nullptr)
        delete[] reinterpret_cast<char *>(acInfo_2);

    delete[] reinterpret_cast<char *>(acInfo_2);

    return 0;
}
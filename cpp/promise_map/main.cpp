#include <chrono>
#include <cstring>
#include <future>
#include <iostream>
#include <map>
// #include <memory>
#include <thread>
#include <tuple>
#include <vector>

using namespace std::chrono_literals;

#pragma pack(push, 1)
typedef struct
{
    uint32_t session_ix;
    uint32_t rcs_tick;
} M__UI__RCS__ROUTE_LIST_REQ;
#define M__UI__RCS__ROUTE_LIST_REQ__MAX_SZ (sizeof(M__UI__RCS__ROUTE_LIST_REQ))
#define MID__UI__RCS__ROUTE_LIST_REQ       (0x4d | 0xcd | 0x1504)

typedef struct
{
    char route_list[64];
} REC__UI__RCS__ROUTE_LIST_ACK;
#define REC__UI__RCS__ROUTE_LIST_ACK__MAX_NO (1000)

typedef struct
{
    uint32_t result;
    uint32_t session_ix;
    uint32_t rcs_tick;
    uint32_t route_list_no;
    REC__UI__RCS__ROUTE_LIST_ACK route_list[0];
} M__UI__RCS__ROUTE_LIST_ACK;
#define M__UI__RCS__ROUTE_LIST_ACK__MAX_SZ                                                                                  \
    (sizeof(M__UI__RCS__ROUTE_LIST_ACK) + sizeof(REC__UI__RCS__ROUTE_LIST_ACK) * REC__UI__RCS__ROUTE_LIST_ACK__MAX_NO)
#define MID__UI__RCS__ROUTE_LIST_ACK (0x4d | 0xcd | 0x15a4)
#pragma pack(pop)

#define PID__PSS__sv (0x0012)

using MessagePromiseMap = std::map<std::tuple<uint32_t, uint32_t, uint32_t>, std::promise<std::vector<char>> *>;

class Requester
{
public:
    Requester(std::shared_ptr<MessagePromiseMap> promise_map)
        : promise_map_(promise_map)
    {
        t_ = std::thread(&Requester::Request, this);
    }
    ~Requester()
    {
        t_.join();
    }
    void Request(void)
    {
        std::vector<char> mid_request_buffer(sizeof(M__UI__RCS__ROUTE_LIST_REQ));
        std::vector<char> mid_response_buffer;

        M__UI__RCS__ROUTE_LIST_REQ *mid_request = reinterpret_cast<M__UI__RCS__ROUTE_LIST_REQ *>(mid_request_buffer.data());
        mid_request->session_ix = 1;
        mid_request->rcs_tick = 123456;

        if (!ForwardRequest<M__UI__RCS__ROUTE_LIST_REQ>(mid_response_buffer, mid_request_buffer,
                                                        MID__UI__RCS__ROUTE_LIST_REQ, PID__PSS__sv))
        {
            return;
        }

        auto mid_response = reinterpret_cast<M__UI__RCS__ROUTE_LIST_ACK *>(mid_response_buffer.data());
        std::cout << "result: " << mid_response->result << std::endl
                  << "session_ix: " << mid_response->session_ix << std::endl
                  << "rcs_tick: " << mid_response->rcs_tick << std::endl
                  << "route_list_no: " << mid_response->route_list_no << std::endl;
        for (int i = 0; i < mid_response->route_list_no; i++)
        {
            std::cout << "route_list[" << i << "]: " << mid_response->route_list[i].route_list << std::endl;
        }
    }

    template <typename RequestType>
    bool ForwardRequest(std::vector<char> &response_buf, const std::vector<char> &request_buf, const uint32_t &request_mid,
                        const uint16_t &dpid, const std::chrono::milliseconds &timeout = 3s)
    {
        auto *request_msg = reinterpret_cast<const RequestType *>(request_buf.data());
        std::cout << "req type: " << typeid(RequestType).name() << std::endl
                  << "req session_ix: " << request_msg->session_ix << std::endl
                  << "req rcs_tick: " << request_msg->rcs_tick << std::endl;

        // ami_send_msg(mid, request_buf.data(), request_buf.size(), dpid, 0);
        std::cout << "send msg" << std::endl << "mid: " << request_mid << std::endl << "dpid: " << dpid << std::endl;

        std::promise<std::vector<char>> promise;
        auto future = promise.get_future();
        auto promise_key = std::make_tuple(request_mid, request_msg->session_ix, request_msg->rcs_tick);
        promise_map_->insert({ promise_key, &promise });

        if (future.wait_for(timeout) == std::future_status::timeout)
        {
            std::cout << "timeout" << std::endl;
            this->promise_map_->erase(promise_key);
            return false;
        }

        this->promise_map_->erase(promise_key);
        response_buf.swap(future.get());

        return true;
    }

private:
    std::shared_ptr<MessagePromiseMap> promise_map_;

    std::thread t_;
};

class PacketManager
{
public:
    PacketManager()
    {
        promise_map_ = std::make_shared<MessagePromiseMap>();
        requester_ = std::make_shared<Requester>(promise_map_);
    }
    void Receive(uint32_t session_ix, uint32_t rcs_tick, uint32_t record_size)
    {
        auto len = sizeof(M__UI__RCS__ROUTE_LIST_ACK) + (record_size * sizeof(REC__UI__RCS__ROUTE_LIST_ACK));
        uint8_t buf[len];
        auto route_list = reinterpret_cast<M__UI__RCS__ROUTE_LIST_ACK *>(buf);
        route_list->result = 0;
        route_list->session_ix = session_ix;
        route_list->rcs_tick = rcs_tick;
        route_list->route_list_no = record_size;
        for (int i = 0; i < route_list->route_list_no; i++)
        {
            std::sprintf(route_list->route_list[i].route_list, "route_%d", i);
        }

        if (auto it = promise_map_->find({ MID__UI__RCS__ROUTE_LIST_REQ, session_ix, rcs_tick }); it != promise_map_->end())
        {
            it->second->set_value(std::vector<char>(reinterpret_cast<char *>(buf), reinterpret_cast<char *>(buf) + len));
        }
        else
        {
            std::cout << "not found" << std::endl;
        }
    }

private:
    std::shared_ptr<Requester> requester_;
    std::shared_ptr<MessagePromiseMap> promise_map_;
};

int main()
{
    PacketManager packet_manager;
    auto now = std::chrono::system_clock::now();
    std::cout << "now: " << std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count() << std::endl;
    std::this_thread::sleep_for(2s);
    std::cout << "now: " << std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count() << std::endl;
    std::this_thread::sleep_until(now + 1s);
    std::cout << "now: " << std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count() << std::endl;
    packet_manager.Receive(1, 123456, 10);
    return 0;
}

//
// Created by mani on 9/14/2020.
//

#ifndef RHEA_BYTEFLOW_REGULATOR_SERVER_H
#define RHEA_BYTEFLOW_REGULATOR_SERVER_H 

#include <cstdint>
#include <ctime>
#include <chrono>
#include <thread>
#include <future>
#include <functional>
#include <sentinel/job_manager/client.h>
#include <basket/communication/rpc_factory.h>
#include <rpc/client.h>

#define DEFAULT_INTERVAL 300
#define VARIATION 100
#define STEP 5

namespace rhea{

    enum  struct Alter_Type: int8_t {
        GROW = 1,
        SHRINK = -1,
    };

    class ByteFlow_Regulator_Server {
    private:
        // make all these maps against job_id
        time_t interval;
        uint_fast16_t variation;
        uint_fast64_t step;
        std::shared_ptr<RPC> client_rpc_;

        std::unordered_map<uint16_t, uint_fast32_t> in_rate_map;
        std::unordered_map<uint16_t, uint_fast32_t> out_rate_map;
        bool AlterCollector(uint16_t job_id, uint_fast64_t out_rate, uint_fast64_t in_rate);
        bool AlterTransformers(uint16_t job_id, uint_fast64_t out_rate, uint_fast64_t in_rate);
        bool AlterNodes(uint16_t job_id, uint_fast64_t out_rate, uint_fast64_t in_rate);
        void RunInternal(std::future<void> futureObj);
    public:
        void Run(std::future<void> futureObj);
        void SetInRate(uint16_t job_id, uint_fast32_t in_rate);
        void SetOutRate(uint16_t job_id, uint_fast32_t out_rate);

        ByteFlow_Regulator_Server(){
            interval = DEFAULT_INTERVAL;
            variation = VARIATION;
            step = STEP;
            SENTINEL_CONF->ConfigureByteflowRegulatorServer();
            auto basket=BASKET_CONF;
            client_rpc_=basket::Singleton<RPCFactory>::GetInstance()->GetRPC(BASKET_CONF->RPC_PORT);
//            std::function<void(uint16_t, uint_fast32_t)> functionSetInRate(std::bind(&rhea::ByteFlow_Regulator_Server::SetInRate, this, std::placeholders::_1, std::placeholders::_2));
//            client_rpc_->bind("SetInRate", functionSetInRate);
//
//            std:function<void(uint16_t, uint_fast32_t)> functionSetOutRate(std::bind(&rhea::ByteFlow_Regulator_Server::SetOutRate, this, std::placeholders::_1, std::placeholders::_2));
//            client_rpc_->bind("setOutRate", functionSetOutRate);
        }
    };
}

#endif //RHEA_JOB_MANAGER_H

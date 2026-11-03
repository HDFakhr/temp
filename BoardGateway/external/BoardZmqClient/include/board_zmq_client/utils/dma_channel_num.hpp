#pragma once

#include <string>

namespace nooyan::zmq_client::utils
{
    struct Dma
    {
        enum class ChannelType: int {
            WB,
            NB,
            NONE
        };

        template<ChannelType T, int N, int ADC>
        struct ChannelNum {
            static constexpr auto value{ -1 };
        };

        template<>
        struct ChannelNum<ChannelType::NB, 0, 0> {
            static constexpr auto value{ 0 };
        };

        template<>
        struct ChannelNum<ChannelType::WB, 0, 0> {
            static constexpr auto value{ 1 };
        };

        template<>
        struct ChannelNum<ChannelType::NB, 0, 1> {
            static constexpr auto value{ 2 };
        };

        template<>
        struct ChannelNum<ChannelType::WB, 0, 1> {
            static constexpr auto value{ 3 };
        };
        
        template<>
        struct ChannelNum<ChannelType::NB, 1, 0> {
            static constexpr auto value{ 4 };
        };

        template<>
        struct ChannelNum<ChannelType::NB, 1, 1> {
            static constexpr auto value{ 5 };
        };

        template<>
        struct ChannelNum<ChannelType::NB, 2, 0> {
            static constexpr auto value{ 6 };
        };

        template<>
        struct ChannelNum<ChannelType::NB, 2, 1> {
            static constexpr auto value{ 7 };
        };

        template<>
        struct ChannelNum<ChannelType::NB, 3, 0> {
            static constexpr auto value{ 8 };
        };

        template<>
        struct ChannelNum<ChannelType::NB, 3, 1> {
            static constexpr auto value{ 9 };
        };

        template<int ADC>
        static constexpr int get_channel_num_for_adc(ChannelType t, int num = 0) {
            if (t == ChannelType::NB) {
                constexpr auto type{ ChannelType::NB };
                switch (num)
                {
                case 0:
                    return ChannelNum<type, 0, ADC>::value;
                case 1:
                    return ChannelNum<type, 1, ADC>::value;
                case 2:
                    return ChannelNum<type, 2, ADC>::value;
                case 3:
                    return ChannelNum<type, 3, ADC>::value;
                }
            }
            if (t == ChannelType::WB) {
                constexpr auto type{ ChannelType::NB };
                return ChannelNum<type, 0, ADC>::value;
            }
            return ChannelNum<ChannelType::NONE, 0, ADC>::value;
        }

        static constexpr int get_channel_num(int adc, ChannelType t, int num = 0) {
            if (adc == 0)
                get_channel_num_for_adc<0>(t, num);
            if (adc == 1)
                get_channel_num_for_adc<1>(t, num);
            return get_channel_num_for_adc<-1>(ChannelType::NONE);
        }
    };
}
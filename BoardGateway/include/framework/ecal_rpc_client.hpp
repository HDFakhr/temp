#pragma once

#include <ecal/ecal.h>
#include <msgpack.hpp>
#include <memory>
#include <type_traits>
#include <exception>
#include <stdexcept>
#include <string>
#include <sstream>

namespace SERVICE_NAMESPACE_PREFIX::framework
{
    class EcalRpcClient {
    public:
        using service_client_t = eCAL::CServiceClient;
        using service_response_vec_t = eCAL::ServiceResponseVecT;
        using service_response_t = eCAL::SServiceResponse;

        EcalRpcClient(std::string service_name) 
            : _service{ std::make_unique<service_client_t>(service_name) }
        {}

        template<typename Out_, typename M_, typename In_>
        typename std::conditional<std::is_void<Out_>::value, void, std::optional<std::decay_t<Out_>>>::type 
        call(M_&& method, In_&& input, int timeout = -1) {
            using InputType = std::decay_t<In_>;
            using OutputType = std::decay_t<Out_>;

            service_response_vec_t responses;
            if constexpr (std::is_same<InputType, std::string>())
                _service->Call(method, input, timeout, &responses);
            else
                _service->Call(method, pack(std::forward<In_>(input)), timeout, &responses);
            service_response_t& response{ responses.at(0) };
            
            if (response.call_state == call_state_failed)
                throw std::runtime_error(response.error_msg);

            if constexpr (std::is_void<OutputType>::value) {
                return;
            } else {
                if (response.response.empty())
                    return {};
                if constexpr (std::is_same<OutputType, std::string>())
                    return std::make_optional(response.response);
                else
                    return std::make_optional(unpack_as<Out_>(response.response));
            }
        }

        template<typename Out_, typename M_>
        typename std::conditional<std::is_void<Out_>::value, void, std::optional<std::decay_t<Out_>>>::type
        call(M_&& method, int timeout = -1) {
            using OutputType = std::decay_t<Out_>;

            service_response_vec_t responses;
            _service->Call(method, "", timeout, &responses);
            service_response_t& response{ responses.at(0) };
        
            if (response.call_state == call_state_failed)
                throw std::runtime_error(response.error_msg);
            
            if constexpr (std::is_void<OutputType>::value) {
                return;
            } else {
                if (response.response.empty())
                    return {};
                if constexpr (std::is_same<OutputType, std::string>())
                    return std::make_optional(response.response);
                else
                    return std::make_optional(unpack_as<Out_>(response.response));
            }
        }

    private:
        std::unique_ptr<service_client_t> _service{ nullptr };

        template<typename T, typename F>
        static auto unpack_as(F&& s) -> T {
            T _;
            auto oh{ msgpack::unpack(s.data(), s.size()) };
            oh.get().convert(_);
            return _;
        }

        template<typename T>
        static std::string pack(T&& d) {
            std::stringstream ss;
            msgpack::pack(ss, d);
            return ss.str();
        }
    };
}
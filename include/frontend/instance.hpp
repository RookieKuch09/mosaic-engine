#pragma once

namespace Mosaic::Frontend
{
    class Instance
    {
    public:
        Instance();
        virtual ~Instance() = default;

        [[nodiscard]] static Instance* Create();

    private:
    };
}
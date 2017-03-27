/*
    Initial author: Convery
    Started: 2017-03-28
    License: MIT
*/

#pragma once
#include <cstdint>
#include <functional>

namespace Hooking
{
    #define EXTENDEDHOOKDECL(Basehook)                                  \
    template <typename Signature, typename ...Arguments>                \
    struct Basehook ##Ex : public Basehook                              \
    {                                                                   \
        std::function<Signature> Function;                              \
        virtual bool Installhook(void *Location, void *Target) override \
        {                                                               \
            Function = *(Signature *)Location;                          \
            Basehook::Installhook(Location, Target);                    \
        }                                                               \
    }                                                                   \
    
    // Basic interface for hooks.
    struct IHook
    {
        uint8_t Savedcode[20]{};
        void *Savedlocation;
        void *Savedtarget;

        virtual bool Removehook() = 0;
        virtual bool Installhook(void *Location, void *Target) = 0;
        virtual bool Reinstall() { return Installhook(Savedlocation, Savedtarget); };
    };

    // A dumb hook that just inserts a jump.
    struct Stomphook : public IHook
    {
        virtual bool Removehook() override;
        virtual bool Installhook(void *Location, void *Target) override;
    };
    EXTENDEDHOOKDECL(Stomphook);

    // A dumb hook that just inserts a call.
    struct Callhook : public IHook
    {
        virtual bool Removehook() override;
        virtual bool Installhook(void *Location, void *Target) override;
    };
    EXTENDEDHOOKDECL(Callhook);
}

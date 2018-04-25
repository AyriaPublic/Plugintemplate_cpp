/*
    Initial author: Convery (tcn@ayria.se)
    Started: 08-01-2018
    License: MIT
    Notes:
        A hook takes control over code-execution and can
        redirect it to your function instead of the apps.
*/

#pragma once
#include "../Stdinclude.hpp"

namespace Hooking
{
    // Call the hooked function, maybe wrap in a a mutex.
    #define Callhook_noret(Hook, ...) { Hook.Remove(); Hook.Function(__VA_ARGS__); Hook.Reinstall(); }
    #define Callhook_ret(Hook, Result, ...) { Hook.Remove(); Result = Hook.Function(__VA_ARGS__); Hook.Reinstall(); }

    // Insert a jump at the address, optionally follows other jumps.
    template <typename Signature = void(void)>
    struct Stomphook
    {
        std::function<Signature> Function;
        uint8_t Savedtext[14]{};
        void *Savedlocation;
        void *Savedtarget;

        void Remove()
        {
            auto Protection = Memprotect::Unprotectrange(Savedlocation, 14);
            {
                std::memcpy(Savedlocation, Savedtext, 14);
            }
            Memprotect::Protectrange(Savedlocation, 14, Protection);
        }
        void Reinstall() { Install(Savedlocation, Savedtarget); }
        static Stomphook Install(void *Location, void *Target, bool Followjumps = true)
        {
            uint8_t *Address = (uint8_t *)Location;
            Stomphook<Signature> Localhook;

            // Check the first byte and increment the address.
            while (Followjumps)
            {
                switch (*(uint8_t *)Address)
                {
                    case 0xE9:
                    {
                        Address += sizeof(uint8_t);
                        Address += *(uint32_t *)Address;
                        Address += sizeof(uint32_t);
                        break;
                    }
                    case 0xFF:
                    {
                        if (*(uint16_t *)Address == 0x25FF)
                        {
                            Address += sizeof(uint16_t);
                            Address += *(uint32_t *)Address;
                            Address += sizeof(uint32_t);
                            Address = (uint8_t *)(*(size_t *)(Address));
                            break;
                        }
                        Followjumps = false;
                        break;
                    }

                    /*
                        TODO(Convery):
                        Add more cases.
                    */

                    default: Followjumps = false;
                }
            }

            // Save the current information for hook removal.
            std::memcpy(Localhook.Savedtext, Address, 14);
            Localhook.Function = *(Signature *)Address;
            Localhook.Savedlocation = Address;
            Localhook.Savedtarget = Target;

            // Write the opcodes.
            auto Protection = Memprotect::Unprotectrange(Address, 14);
            {
                #if defined(ENVIRONMENT64)
                std::memcpy(Address, "\xFF\x25", 2);
                std::memcpy(Address + 2, "\x00\x00\x00\x00", 4);
                std::memcpy(Address + 6, &Target, sizeof(void *));
                #else
                std::memcpy(Address, "\xE9", 1);
                Target = (uint8_t *)((size_t)Target - (size_t)Address - 5);
                std::memcpy(Address + 1, &Target, sizeof(void *));
                #endif
            }
            Memprotect::Protectrange(Address, 14, Protection);

            return Localhook;
        }
        static Stomphook Install(std::uintptr_t Location, std::uintptr_t Target, bool Followjumps = true)
        {
            return Install(reinterpret_cast<void *>(Location), reinterpret_cast<void *>(Target), Followjumps);
        }
        static Stomphook Install(void *Location, std::uintptr_t Target, bool Followjumps = true)
        {
            return Install(Location, reinterpret_cast<void *>(Target), Followjumps);
        }
        static Stomphook Install(std::uintptr_t Location, void *Target, bool Followjumps = true)
        {
            return Install(reinterpret_cast<void *>(Location), Target, Followjumps);
        }
    };

    // Replace a block of data with NOP instructions.
    template <size_t Size>
    struct NOPPatch
    {
        uint8_t Savedtext[Size]{};
        void *Savedlocation;

        void Remove()
        {
            auto Protection = Memprotect::Unprotectrange(Savedlocation, Size);
            {
                std::memcpy(Savedlocation, Savedtext, Size);
            }
            Memprotect::Protectrange(Savedlocation, Size, Protection);
        }
        static NOPPatch Install(void *Location)
        {
            NOPPatch<Size> Localpatch;

            // Save the current information for patch removal.
            std::memcpy(Localpatch.Savedtext, Location, Size);
            Localpatch.Savedlocation = Location;

            auto Protection = Memprotect::Unprotectrange(Location, Size);
            {
                std::memset(Location, 0x90, Size);
            }
            Memprotect::Protectrange(Location, Size, Protection);

            return Localpatch;
        }
        static NOPPatch Install(std::uintptr_t Location)
        {
            return Install(reinterpret_cast<void *>(Location));
        }
    };
}

/*
    Usage example:

    Hooking::Stomphook<decltype(std::memcpy)> Stomphook;
    void *Stompfunc(void *Src, void *Dst, size_t Size)
    {
        void *Result{};

        printf("Copying %llX bytes!\n", Size);
        Callhook_ret(Stomphook, Result, Src, Dst, Size);

        return Result;
    }

    Stomphook = Hooking::Stomphook<decltype(std::memcpy)>::Install(std::memcpy, Stompfunc);
    Patch = Hooking::NOPPatch<sizeof(std::memcpy)>::Install(std::memcpy);
*/

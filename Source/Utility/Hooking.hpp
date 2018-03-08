/*
    Initial author: Convery (tcn@hedgehogscience.com)
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
        uint8_t Savedtext[7]{};
        void *Savedlocation;
        void *Savedtarget;

        void Remove()
        {
            auto Protection = Memprotect::Unprotectrange(Savedlocation, 7);
            {
                std::memcpy(Savedlocation, Savedtext, 7);
            }
            Memprotect::Protectrange(Savedlocation, 7, Protection);
        }
        void Reinstall() { Install(Savedlocation, Savedtarget); }
        static Stomphook<Signature> Install(void *Location, void *Target, bool Followjumps = true)
        {
            uint8_t *Address = (uint8_t *)Location;
            Stomphook<Signature> Localhook;

            // Check if there's already a hook in-place.
            while (Followjumps)
            {
                // x86_32 short jump.
                if (Address[0] == 0xE9)
                {
                    Address += *(uint32_t *)&Address[1] - 5;
                    continue;
                }

                // x86_64 relative jump.
                if (Address[0] == 0xFF && Address[1] == 0x25)
                {
                    Address += *(uint32_t *)&Address[2] - 7;
                    continue;
                }

                break;
            }

            // Save the current information for hook removal.
            std::memcpy(Localhook.Savedtext, Address, 7);
            Localhook.Function = *(Signature *)Target;
            Localhook.Savedlocation = Address;
            Localhook.Savedtarget = Target;

            // Write the opcodes.
            auto Protection = Memprotect::Unprotectrange(Address, 7);
            {
                uint32_t Relative = uint32_t((uint8_t *)Target - Address + 7);
                uint8_t Opcodes[2]{ 0xFF, 0x25 }; // JMP EIP/RIP + Address
                assert((uint8_t *)Target - Address < INT32_MAX);

                std::memcpy(&Address[0], Opcodes, 2);
                std::memcpy(&Address[2], &Relative, sizeof(uint32_t));
            }
            Memprotect::Protectrange(Address, 7, Protection);

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

    // Replace a call to a function with our own.
    template <typename Signature = void(void)>
    struct Callhook
    {
        std::function<Signature> Function;
        uint8_t Savedtext[7]{};
        void *Savedlocation;
        void *Savedtarget;

        void Remove()
        {
            auto Protection = Memprotect::Unprotectrange(Savedlocation, 7);
            {
                std::memcpy(Savedlocation, Savedtext, 7);
            }
            Memprotect::Protectrange(Savedlocation, 7, Protection);
        }
        void Reinstall() { Install(Savedlocation, Savedtarget); }
        static Callhook Install(void *Location, void *Target)
        {
            uint8_t *Address = (uint8_t *)Location;
            Callhook<Signature> Localhook;

            // Check if there's a call at the address.
            assert(Address[0] != 0xFF && Address[1] != 0x15);

            // Save the current information for hook removal.
            Localhook.Function = *(Signature *)uint64_t(*(uint32_t *)&Address[2] - 7);
            std::memcpy(Localhook.Savedtext, Address, 7);
            Localhook.Savedlocation = Address;
            Localhook.Savedtarget = Target;

            // Write the opcodes.
            auto Protection = Memprotect::Unprotectrange(Address, 7);
            {
                uint32_t Relative = uint32_t((uint8_t *)Target - Address + 7);
                uint8_t Opcodes[2]{ 0xFF, 0x15 }; // CALL EIP/RIP + Address.
                assert((uint8_t *)Target - Address < INT32_MAX);

                std::memcpy(&Address[0], Opcodes, 2);
                std::memcpy(&Address[2], &Relative, sizeof(uint32_t));
            }
            Memprotect::Protectrange(Address, 7, Protection);

            return Localhook;
        }
        static Callhook Install(std::uintptr_t Location, std::uintptr_t Target)
        {
            return Install(reinterpret_cast<void *>(Location), reinterpret_cast<void *>(Target));
        }
        static Callhook Install(void *Location, std::uintptr_t Target)
        {
            return Install(Location, reinterpret_cast<void *>(Target));
        }
        static Callhook Install(std::uintptr_t Location, void *Target)
        {
            return Install(reinterpret_cast<void *>(Location), Target);
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

    Hooking::Callhook<decltype(std::memcpy)> Callhook;
    void *Callfunc(void *Src, void *Dst, size_t Size)
    {
        printf("Copying %llX bytes!\n", Size);
        return Callhook.Function(Src, Dst, Size);
    }

    Stomphook = Hooking::Stomphook<decltype(std::memcpy)>::Install(std::memcpy, Stompfunc);
    Callhook = Hooking::Callhook<decltype(std::memcpy)>::Install(0x07001000, Callfunc);
    Patch = Hooking::NOPPatch<sizeof(std::memcpy)>::Install(std::memcpy);
*/

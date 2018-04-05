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
        uint8_t Savedtext[16]{};
        void *Savedlocation;
        void *Savedtarget;

        void Remove()
        {
            auto Protection = Memprotect::Unprotectrange(Savedlocation, 16);
            {
                std::memcpy(Savedlocation, Savedtext, 16);
            }
            Memprotect::Protectrange(Savedlocation, 16, Protection);
        }
        void Reinstall() { Install(Savedlocation, Savedtarget); }
        static Stomphook Install(void *Location, void *Target, bool Followjumps = true)
        {
            uint8_t *Address = (uint8_t *)Location;
            Stomphook<Signature> Localhook;

            /*
                TODO(Convery):
                Disassemble the first bytes and check for jumps.
                Then follow them so we don't overwrite hooks.
            */

            // Save the current information for hook removal.
            std::memcpy(Localhook.Savedtext, Address, 16);
            Localhook.Function = *(Signature *)Address;
            Localhook.Savedlocation = Address;
            Localhook.Savedtarget = Target;

            // Write the opcodes.
            auto Protection = Memprotect::Unprotectrange(Address, 16);
            {
                std::string Opcodes;

                // PUSH EAX/RAX
                Opcodes.append("\x50", 1);
                // QWORD modifier
                if constexpr(sizeof(void *) == 8) Opcodes.append("\x48", 1);
                // MOV EAX/RAX
                Opcodes.append("\xB8", 1);
                // Address to jump to.
                Opcodes.append((char *)&Target, sizeof(void *));
                // QWORD modifier
                if constexpr(sizeof(void *) == 8) Opcodes.append("\x48", 1);
                // XCHG EAX/RAX, [ESP/RSP]
                Opcodes.append("\x87\x04\x24", 3);
                // RET
                Opcodes.append("\xC3", 1);

                std::memcpy(Address, Opcodes.data(), Opcodes.size());
            }
            Memprotect::Protectrange(Address, 16, Protection);

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

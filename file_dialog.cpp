#include "file_dialog.h"

#include <imgui.h>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <cctype>

namespace fs = std::filesystem;

namespace {
    struct Entry { fs::path path; bool isDir; };

    bool hasImageExt(const fs::path& p) {
        if (!p.has_extension()) return false;
        std::string ext = p.extension().string();
        std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c){ return (char)std::tolower(c); });
        return (ext == ".png" || ext == ".jpg" || ext == ".jpeg");
    }

    std::vector<Entry> readDir(const fs::path& dir) {
        std::vector<Entry> out;
        try {
            for (auto& de : fs::directory_iterator(dir)) {
                out.push_back({de.path(), de.is_directory()});
            }
        } catch (...) { /* brak dostępu / błędy IO */ }
        std::sort(out.begin(), out.end(), [](const Entry& a, const Entry& b){
            if (a.isDir != b.isDir) return a.isDir && !b.isDir;
            return a.path.filename().string() < b.path.filename().string();
        });
        return out;
    }

    // --- Stan dialogu (prywatny w tym module) ---
    bool g_open = false;
    fs::path g_currentDir;
    std::vector<Entry> g_entries;
    char g_pathBuf[1024] = {0};
    std::optional<std::string> g_result; // ustawiany po potwierdzeniu
}

namespace imgpicker {
    void Open(const std::string& startDir) {
        g_open = true;
        if (!startDir.empty() && fs::exists(startDir) && fs::is_directory(startDir))
            g_currentDir = fs::weakly_canonical(startDir);
        else
            g_currentDir = fs::current_path();

        g_entries = readDir(g_currentDir);
        std::string s = g_currentDir.string();
        if (s.size() >= sizeof(g_pathBuf)) s.resize(sizeof(g_pathBuf)-1);
        std::strncpy(g_pathBuf, s.c_str(), sizeof(g_pathBuf));
        g_pathBuf[sizeof(g_pathBuf)-1] = '';
        } 
        
        else {
                    const bool isImage = hasImageExt(e.path);
                    if (!isImage) ImGui::BeginDisabled();
                    bool clicked = ImGui::Selectable(name.c_str(), false, ImGuiSelectableFlags_AllowDoubleClick);
                    if (!isImage) ImGui::EndDisabled();

                    if (isImage && clicked && ImGui::IsMouseDoubleClicked(0)) {
                        g_result = e.path.string();
                        g_open = false; // zamknięcie okna po wyborze
                    }
                }
            }
            ImGui::EndChild();

            // Pasek akcji
            ImGui::Separator();
            if (ImGui::Button("Anuluj")) { g_open = false; g_result.reset(); }
            ImGui::SameLine();
            if (ImGui::Button("Wybierz")) {
                // Jeśli wklejono pełną ścieżkę do pliku, pozwól zatwierdzić ręcznie
                fs::path p(g_pathBuf);
                if (fs::exists(p) && fs::is_regular_file(p) && hasImageExt(p)) {
                    g_result = p.string();
                    g_open = false;
                }
            }
        }
        ImGui::End();

        return g_result.has_value();
    }

    std::optional<std::string> TakeResult() {
        if (!g_result) return std::nullopt;
        auto tmp = std::move(g_result);
        g_result.reset();
        return tmp;
    }
}

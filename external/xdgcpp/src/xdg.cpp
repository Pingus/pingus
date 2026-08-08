// Copyright (C) 2015 Thomas Voß <thomas.voss.bochum@gmail.com>
//               2022 Ingo Ruhnke <grumbel@gmail.com>
//
// This library is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <xdg.h>

#include <algorithm>
#include <cstdlib>
#include <iterator>
#include <stdexcept>
#include <string>

namespace fs = std::filesystem;

namespace
{

std::vector<std::string> path_split(const std::string& path)
{
  std::vector<std::string> result;
  std::size_t end = path.find(':');

  std::size_t start = 0;
  while (end != std::string::npos)
  {
    result.push_back(path.substr(start, end - start));

    start = end + 1;
    end = path.find(':', start);
  }

  result.push_back(path.substr(start, end - start));

  return result;
}

fs::path throw_if_not_absolute(const fs::path& p)
{
    if (p.has_root_directory())
        return p;

    throw std::runtime_error{"Directores MUST be absolute."};
}

namespace env
{
std::string get(const std::string& key, const std::string& default_value)
{
    if (const char* value = std::getenv(key.c_str()))
        return value;
    return default_value;
}

std::string get_or_throw(const std::string& key)
{
    if (const char* value = std::getenv(key.c_str()))
    {
        return value;
    }

    throw std::runtime_error{key + " not set in environment"};
}

constexpr const char* xdg_data_home{"XDG_DATA_HOME"};
constexpr const char* xdg_data_dirs{"XDG_DATA_DIRS"};
constexpr const char* xdg_config_home{"XDG_CONFIG_HOME"};
constexpr const char* xdg_config_dirs{"XDG_CONFIG_DIRS"};
constexpr const char* xdg_cache_home{"XDG_CACHE_HOME"};
constexpr const char* xdg_state_home{"XDG_STATE_HOME"};
constexpr const char* xdg_runtime_dir{"XDG_RUNTIME_DIR"};
}

namespace impl
{
class BaseDirSpecification : public xdg::BaseDirSpecification
{
public:
    static const BaseDirSpecification& instance()
    {
        static const BaseDirSpecification spec;
        return spec;
    }

    BaseDirSpecification()
    {
    }

    const xdg::Data& data() const override
    {
        return data_;
    }

    const xdg::Config& config() const override
    {
        return config_;
    }

    const xdg::State& state() const override
    {
        return state_;
    }

    const xdg::Cache& cache() const override
    {
        return cache_;
    }

    const xdg::Runtime& runtime() const override
    {
        return runtime_;
    }

private:
    xdg::Data data_;
    xdg::Config config_;
    xdg::State state_;
    xdg::Cache cache_;
    xdg::Runtime runtime_;
};
}
}

fs::path xdg::Data::home() const
{
    auto v = env::get(env::xdg_data_home, "");
    if (v.empty())
        return throw_if_not_absolute(fs::path{env::get_or_throw("HOME")} / ".local" / "share");

    return throw_if_not_absolute(fs::path(v));
}

std::vector<fs::path> xdg::Data::dirs() const
{
    auto v = env::get(env::xdg_data_dirs, "");
    if (v.empty())
        return {fs::path{"/usr/local/share"}, fs::path{"/usr/share"}};

    std::vector<std::string> tokens = path_split(v);
    std::vector<fs::path> result;
    result.reserve(tokens.size());
    for (const auto& token : tokens)
    {
        result.push_back(throw_if_not_absolute(fs::path(token)));
    }
    return result;
}

fs::path xdg::Config::home() const
{
    auto v = env::get(env::xdg_config_home, "");
    if (v.empty())
        return throw_if_not_absolute(fs::path{env::get_or_throw("HOME")} / ".config");

    return throw_if_not_absolute(fs::path(v));
}

std::vector<fs::path> xdg::Config::dirs() const
{
    auto v = env::get(env::xdg_config_dirs, "");
    if (v.empty())
        return {fs::path{"/etc/xdg"}};

    std::vector<std::string> tokens = path_split(v);
    std::vector<fs::path> result;
    for (const auto& token : tokens)
    {
        fs::path p(token);
        result.push_back(throw_if_not_absolute(p));
    }
    return result;
}

fs::path xdg::State::home() const
{
    auto v = env::get(env::xdg_state_home, "");
    if (v.empty())
        return throw_if_not_absolute(fs::path{env::get_or_throw("HOME")} / ".local" / "state");

    return throw_if_not_absolute(fs::path(v));
}

fs::path xdg::Cache::home() const
{
    auto v = env::get(env::xdg_cache_home, "");
    if (v.empty())
        return throw_if_not_absolute(fs::path{env::get_or_throw("HOME")} / ".cache");

    return throw_if_not_absolute(fs::path(v));
}

fs::path xdg::Runtime::dir() const
{
    auto v = env::get(env::xdg_runtime_dir, "");
    if (v.empty())
    {
        // We do not fall back gracefully and instead throw, dispatching to calling
        // code for handling the case of a safe user-specfic runtime directory missing.
        throw std::runtime_error{"Runtime directory not set"};
    }

    return throw_if_not_absolute(fs::path(v));
}

std::shared_ptr<xdg::BaseDirSpecification> xdg::BaseDirSpecification::create()
{
    return std::make_shared<impl::BaseDirSpecification>();
}

const xdg::Data& xdg::data()
{
    return impl::BaseDirSpecification::instance().data();
}

const xdg::Config& xdg::config()
{
    return impl::BaseDirSpecification::instance().config();
}

const xdg::State& xdg::state()
{
    return impl::BaseDirSpecification::instance().state();
}

const xdg::Cache& xdg::cache()
{
    return impl::BaseDirSpecification::instance().cache();
}

const xdg::Runtime& xdg::runtime()
{
    return impl::BaseDirSpecification::instance().runtime();
}

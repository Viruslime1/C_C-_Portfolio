#include "Storage.h"

// --- Реализация методов Storage ---

Id Storage::createUser(const std::string &username, const std::string &displayName, const std::string &bio) {
    Id id = nextUserId++;
    users.emplace(id, User{id, username, displayName, bio});
    return id;
}

std::optional<User> Storage::getUser(Id id) const {
    auto it = users.find(id);
    if(it == users.end()) return std::nullopt;
    return it->second;
}

std::optional<User> Storage::findUserByUsername(const std::string &username) const {
    for (const auto &p : users) {
        if (p.second.username == username) return p.second;
    }
    return std::nullopt;
}

bool Storage::updateUser(Id id, const std::string &displayName, const std::string &bio) {
    auto it = users.find(id);
    if(it == users.end()) return false;
    it->second.displayName = displayName;
    it->second.bio = bio;
    return true;
}

bool Storage::deleteUser(Id id) {
    if(users.erase(id) == 0) return false;
    followers.erase(id);
    following.erase(id);
    for(auto &p : followers) p.second.erase(id);
    for(auto &p : following) p.second.erase(id);

    std::vector<Id> toRemove;
    for (auto &pr : posts) if (pr.second.authorId == id) toRemove.push_back(pr.first);
    for (Id pid : toRemove) posts.erase(pid);

    return true;
}

// --- Follow ---
bool Storage::follow(Id followerId, Id followeeId) {
    if(users.find(followerId) == users.end()) return false;
    if(users.find(followeeId) == users.end()) return false;
    if(followerId == followeeId) return false;
    following[followerId].insert(followeeId);
    followers[followeeId].insert(followerId);
    return true;
}

bool Storage::unfollow(Id followerId, Id followeeId) {
    if(following[followerId].erase(followeeId)) {
        followers[followeeId].erase(followerId);
        return true;
    }
    return false;
}

std::vector<Id> Storage::getFollowers(Id userId) const {
    std::vector<Id> res;
    auto it = followers.find(userId);
    if(it == followers.end()) return res;
    res.insert(res.end(), it->second.begin(), it->second.end());
    return res;
}

std::vector<Id> Storage::getFollowing(Id userId) const {
    std::vector<Id> res;
    auto it = following.find(userId);
    if(it == following.end()) return res;
    res.insert(res.end(), it->second.begin(), it->second.end());
    return res;
}

// --- Posts ---
Id Storage::createPost(Id authorId, const std::string &content) {
    if(users.find(authorId) == users.end()) return -1;
    Id id = nextPostId++;
    posts.emplace(id, Post{id, authorId, content});
    return id;
}

std::optional<Post> Storage::getPost(Id id) const {
    auto it = posts.find(id);
    if(it == posts.end()) return std::nullopt;
    return it->second;
}

bool Storage::deletePost(Id id) {
    return posts.erase(id) > 0;
}

std::vector<Post> Storage::getPostsByUser(Id userId) const {
    std::vector<Post> res;
    for (const auto &pr : posts) {
        if (pr.second.authorId == userId) res.push_back(pr.second);
    }
    std::sort(res.begin(), res.end(), [](const Post &a, const Post &b){ return a.timestamp > b.timestamp; });
    return res;
}

// --- Save/Load ---
bool Storage::saveToDirectory(const std::string &dir) const {
    try {
        std::ofstream uf(dir + "/users.txt", std::ios::trunc);
        for (const auto &p : users) {
            uf << p.second.id << "|" << escape(p.second.username) << "|" << escape(p.second.displayName)
               << "|" << escape(p.second.bio) << "\n";
        }
        uf.close();

        std::ofstream pf(dir + "/posts.txt", std::ios::trunc);
        for (const auto &p : posts) {
            pf << p.second.id << "|" << p.second.authorId << "|" << p.second.timestamp
               << "|" << escape(p.second.content) << "\n";
        }
        pf.close();

        std::ofstream ff(dir + "/follows.txt", std::ios::trunc);
        for (const auto &p : following) {
            Id follower = p.first;
            for (Id followee : p.second) {
                ff << follower << "|" << followee << "\n";
            }
        }
        ff.close();

        std::ofstream meta(dir + "/meta.txt", std::ios::trunc);
        meta << nextUserId << "|" << nextPostId << "\n";
        meta.close();
    } catch (...) {
        return false;
    }
    return true;
}

bool Storage::loadFromDirectory(const std::string &dir) {
    try {
        users.clear(); posts.clear(); followers.clear(); following.clear();

        std::ifstream uf(dir + "/users.txt");
        if(uf.is_open()) {
            std::string line;
            while (std::getline(uf, line)) {
                auto parts = split(line, '|', 4);
                if(parts.size() < 4) continue;
                Id id = std::stoll(parts[0]);
                User u{id, unescape(parts[1]), unescape(parts[2]), unescape(parts[3])};
                users.emplace(id, std::move(u));
            }
            uf.close();
        }

        std::ifstream pf(dir + "/posts.txt");
        if(pf.is_open()) {
            std::string line;
            while (std::getline(pf, line)) {
                auto parts = split(line, '|', 4);
                if(parts.size() < 4) continue;
                Id id = std::stoll(parts[0]);
                Id author = std::stoll(parts[1]);
                std::time_t ts = static_cast<std::time_t>(std::stoll(parts[2]));
                Post po; po.id = id; po.authorId = author; po.timestamp = ts; po.content = unescape(parts[3]);
                posts.emplace(id, std::move(po));
            }
            pf.close();
        }

        std::ifstream ff(dir + "/follows.txt");
        if(ff.is_open()) {
            std::string line;
            while (std::getline(ff, line)) {
                auto parts = split(line, '|', 2);
                if(parts.size() < 2) continue;
                Id follower = std::stoll(parts[0]);
                Id followee = std::stoll(parts[1]);
                following[follower].insert(followee);
                followers[followee].insert(follower);
            }
            ff.close();
        }

        std::ifstream meta(dir + "/meta.txt");
        if(meta.is_open()) {
            std::string line;
            if(std::getline(meta, line)) {
                auto parts = split(line, '|', 2);
                if(parts.size() >= 2) {
                    nextUserId = std::stoll(parts[0]);
                    nextPostId = std::stoll(parts[1]);
                }
            }
            meta.close();
        }
    } catch (...) {
        return false;
    }
    return true;
}

// --- Utility functions ---
std::string Storage::escape(const std::string &s) {
    std::string out;
    for(char c : s) {
        if(c == '\\') out += "\\\\";
        else if(c == '\n') out += "\\n";
        else if(c == '|') out += "\\p";
        else out += c;
    }
    return out;
}

std::string Storage::unescape(const std::string &s) {
    std::string out;
    for(size_t i=0;i<s.size();++i) {
        if(s[i] == '\\' && i+1 < s.size()) {
            char nx = s[i+1];
            if(nx == '\\') { out += '\\'; ++i; }
            else if(nx == 'n') { out += '\n'; ++i; }
            else if(nx == 'p') { out += '|'; ++i; }
            else out += s[i];
        } else out += s[i];
    }
    return out;
}

std::vector<std::string> Storage::split(const std::string &s, char delim, size_t maxParts) {
    std::vector<std::string> res;
    std::string cur;
    for(char c : s) {
        if(c == delim && res.size() + 1 < maxParts) {
            res.push_back(cur);
            cur.clear();
        } else cur.push_back(c);
    }
    res.push_back(cur);
    return res;
}

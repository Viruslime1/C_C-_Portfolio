#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <sstream>
#include <ctime>
#include <optional>

// Тип для ID
using Id = long long;

// --- User ---
struct User {
    Id id;
    std::string username;
    std::string displayName;
    std::string bio;

    User() = default;
    User(Id i, std::string u, std::string d, std::string b = "")
        : id(i), username(std::move(u)), displayName(std::move(d)), bio(std::move(b)) {}
};

// --- Post ---
struct Post {
    Id id;
    Id authorId;
    std::string content;
    std::time_t timestamp;

    Post() = default;
    Post(Id i, Id a, std::string c)
        : id(i), authorId(a), content(std::move(c)), timestamp(std::time(nullptr)) {}
};

// --- Storage class ---
class Storage {
private:
    Id nextUserId = 1;
    Id nextPostId = 1;

    std::unordered_map<Id, User> users;
    std::unordered_map<Id, Post> posts;
    std::unordered_map<Id, std::unordered_set<Id>> followers;
    std::unordered_map<Id, std::unordered_set<Id>> following;

public:
    // Методы (объявления, без реализации)
    Id createUser(const std::string &username, const std::string &displayName, const std::string &bio = "");
    std::optional<User> getUser(Id id) const;
    std::optional<User> findUserByUsername(const std::string &username) const;
    bool updateUser(Id id, const std::string &displayName, const std::string &bio);
    bool deleteUser(Id id);

    bool follow(Id followerId, Id followeeId);
    bool unfollow(Id followerId, Id followeeId);
    std::vector<Id> getFollowers(Id userId) const;
    std::vector<Id> getFollowing(Id userId) const;

    Id createPost(Id authorId, const std::string &content);
    std::optional<Post> getPost(Id id) const;
    bool deletePost(Id id);
    std::vector<Post> getPostsByUser(Id userId) const;

    bool saveToDirectory(const std::string &dir) const;
    bool loadFromDirectory(const std::string &dir);

private:
    static std::string escape(const std::string &s);
    static std::string unescape(const std::string &s);
    static std::vector<std::string> split(const std::string &s, char delim, size_t maxParts = SIZE_MAX);
};

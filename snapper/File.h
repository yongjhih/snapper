/*
 * Copyright (c) 2011 Novell, Inc.
 *
 * All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, contact Novell, Inc.
 *
 * To contact Novell about this file by physical or electronic mail, you may
 * find current contact information at www.novell.com.
 */


#ifndef FILE_H
#define FILE_H


#include <string>
#include <vector>


namespace snapper
{
    using std::string;
    using std::vector;


    enum StatusFlags
    {
	CREATED = 1, DELETED = 2, TYPE = 4, CONTENT = 8, PERMISSIONS = 16, USER = 32,
	GROUP = 64
    };

    enum Cmp
    {
	CMP_PRE_TO_POST, CMP_PRE_TO_SYSTEM, CMP_POST_TO_SYSTEM
    };

    enum Location
    {
	LOC_PRE, LOC_POST, LOC_SYSTEM
    };


    class File
    {
    public:

	File(const string& name, unsigned int pre_to_post_status)
	    : name(name), pre_to_post_status(pre_to_post_status), pre_to_system_status(-1),
	      post_to_system_status(-1), rollback(false)
	{}

	const string& getName() const { return name; }

	unsigned int getPreToPostStatus() const { return pre_to_post_status; }
	unsigned int getPreToSystemStatus();
	unsigned int getPostToSystemStatus();

	unsigned int getStatus(Cmp cmp);

	string getAbsolutePath(Location loc);

	bool getRollback() const { return rollback; }
	void setRollback(bool value) { rollback = value; }

	bool doRollback();

    private:

	string name;

	unsigned int pre_to_post_status;
	unsigned int pre_to_system_status; // -1 if invalid
	unsigned int post_to_system_status; // -1 if invalid

	bool rollback;

    };


    inline int operator<(const File& a, const File& b)
    {
	return a.getName() < b.getName();
    }


    class Files
    {
    public:

	Files() : initialized(false) {}

	void assertInit();

	bool doRollback();

	typedef vector<File>::iterator iterator;
	typedef vector<File>::const_iterator const_iterator;

	iterator begin() { return entries.begin(); }
	const_iterator begin() const { return entries.begin(); }

	iterator end() { return entries.end(); }
	const_iterator end() const { return entries.end(); }

	iterator find(const string& name);
	const_iterator find(const string& name) const;

    private:

	void initialize();

	void create();
	bool load();
	bool save();

	bool initialized;

	vector<File> entries;

	friend void append_helper(const string& name, unsigned int status);

    };

}


#endif
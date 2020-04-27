//
//  Utilities.h
//  SimpleGameFramework
//
// Created by Edward Rudd on 2019-08-19.
//
#pragma once

#include <utility>
#include <list>
#include <algorithm>


template<typename T, class P, typename... Args>
static typename std::list<T>::iterator find_or_create(std::list<T>& list, P predicate, Args&& ...args)
{
	auto it = std::find_if_not( list.begin(), list.end(), predicate );
	if (it == list.end()) {
		list.emplace_back( std::forward<Args>( args )... );
		it = --list.end();
	}
	return it;
}
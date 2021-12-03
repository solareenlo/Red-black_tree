/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tayamamo <tayamamo@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/04 00:01:00 by tayamamo          #+#    #+#             */
/*   Updated: 2021/12/04 04:11:35 by tayamamo         ###   ########.fr       */
/* ************************************************************************** */

#include "rbtree.hpp"

int main() {
    ft::rbtree<int> rbt;

    rbt.insertKey(0);
    rbt.insertKey(1);
    rbt.insertKey(2);

    rbt.preorder();
    rbt.inorder();
    rbt.postorder();
    return 0;
}

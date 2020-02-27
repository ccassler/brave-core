/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import * as React from 'react'

import { OrderSummary } from './orderSummary'
import { UseWalletPanel } from './useWalletPanel';

export interface PaymentMethodProps {
  itemName: string
  orderPrice: number
  exchangePrice: string
}

export function PaymentMethodPanel (props: PaymentMethodProps) {
  return <>
    <h1>Payment Method</h1>
    <OrderSummary
      itemName={props.itemName}
      orderPrice={props.orderPrice}
      exchangePrice={props.exchangePrice} />
    <UseWalletPanel />
  </>
}

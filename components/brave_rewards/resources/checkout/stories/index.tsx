/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import * as React from 'react'
import { storiesOf } from '@storybook/react'

import { CheckoutDialog } from '../components/checkoutDialog'
import { PaymentMethodPanel } from '../components/paymentMethodPanel'

function noop() {}

const longItemTitle =
  'Title of the selected item long title long ' +
  'long title wrapped into second line'

storiesOf('Rewards/Checkout', module)
  .add('CheckoutDialog', () => {
    return (
      <CheckoutDialog onClose={noop}>
        Hello world
      </CheckoutDialog>
    )
  })
  .add('PaymentMethodPanel', () => {
    return (
      <CheckoutDialog onClose={noop}>
        <PaymentMethodPanel
          itemName={longItemTitle}
          orderPrice={45.0}
          exchangePrice={'$9.00'} />
      </CheckoutDialog>
    )
  })
